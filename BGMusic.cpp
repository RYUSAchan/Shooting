//BGmusic.cpp

#include "BGMusic.h"

void BGMusic::InitializeBGM()
{
	pBuilder = NULL;
	pMediaCtrl = NULL;
	pMediaSeeking = NULL;
	pBasicAudio = NULL;

	bLoop = FALSE;
	bFading = FALSE;
}

void BGMusic::Release()
{
	RELEASE( pBasicAudio );
	RELEASE( pMediaSeeking );
	RELEASE( pMediaCtrl );
	RELEASE( pBuilder );
}

BGMusic::BGMusic()
{
	InitializeBGM();
}

BGMusic::BGMusic( LPCTSTR filename )
{
	InitializeBGM();
	Load( filename );
}

BGMusic::~BGMusic()
{
	Release();
}

void BGMusic::Load( LPCTSTR filename )
{
	Release();

	HRESULT hr = CoCreateInstance(
		CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder,
		( LPVOID* )&pBuilder );

	if( FAILED( hr ) ) {
		CautionMessage( _T( "errorz" ), _T( "IGraphBuilderの作成に失敗しました。" ) );
		return;
	}

#ifdef _UNICODE
	pBuilder->RenderFile( filename, NULL );
#else
	wchar_t wfname[ MAX_PATH ];
	MultiByteToWideChar( CP_ACP, 0, filename, -1, wfname, MAX_PATH );
	pBuilder->RenderFile( wfname, NULL );
#endif

	pBuilder->QueryInterface( IID_IMediaControl, ( LPVOID* )&pMediaCtrl );
	pBuilder->QueryInterface( IID_IMediaSeeking, ( LPVOID* )&pMediaSeeking );
	pBuilder->QueryInterface( IID_IBasicAudio, ( LPVOID* )&pBasicAudio );
}

void BGMusic::Play( BOOL loop )
{
	if( pMediaCtrl ) { 
		bLoop = loop;
		if( pMediaSeeking ) {
			LONGLONG s = 0;
			pMediaSeeking->SetPositions(
				&s, AM_SEEKING_AbsolutePositioning,
				NULL, AM_SEEKING_NoPositioning );
		}
		pMediaCtrl->Run();
	}
}

void BGMusic::Stop()
{
	if( pMediaCtrl ) { pMediaCtrl->Stop(); }
}

void BGMusic::SetVolume( LONG volume )
{
	if( pBasicAudio ) { pBasicAudio->put_Volume( volume ); }
}

void BGMusic::Exec()
{
	if( bLoop && pMediaSeeking ) {
		LONGLONG s, e;
		pMediaSeeking->GetPositions( &s, &e );
		if( s >= e ) {
			s = 0;
			pMediaSeeking->SetPositions(
				&s, AM_SEEKING_AbsolutePositioning,
				NULL, AM_SEEKING_NoPositioning );
		}
	}

	if( bFading )
	{
		float t = ( float )timeGetTime() - time_s;
		pBasicAudio->put_Volume( ( long )( delta * t + vol_s ) );

		TCHAR str[ 20 ];
		_stprintf_s( str, 20, _T( "%f" ), delta * t + vol_s );

		if( t >= time_e ) { bFading = FALSE; }
	}
}

void BGMusic::Fade( DWORD endtime, LONG volume )
{
	if( !pBasicAudio ) {
		CautionMessage( _T( "errorz" ), _T( "ボリュームの設定ができません。" ) );
		return;
	}

	bFading = TRUE;

	LONG v;
	pBasicAudio->get_Volume( &v );

	vol_s = ( float )v;
	float vol_e = ( float )volume;

	time_s = ( float )timeGetTime();
	time_e = ( float )endtime;

	delta = ( vol_e - vol_s ) / time_e;
}
