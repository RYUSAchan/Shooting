//Sound.cpp

#include "Sound.h"

LPDIRECTSOUND8 Sound::pDSound = NULL;

Sound::Sound()
{
	Reset();
}

Sound::Sound( LPCTSTR filename )
{
	Reset();
	Load( filename );
}

Sound::Sound( const Sound &s )
{
	Reset();
	if( pDSound ) { pDSound->DuplicateSoundBuffer( s.pDSBuffer, &pDSBuffer ); }
}

Sound& Sound::operator=( const Sound &s )
{
	RELEASE( pDSBuffer );

	Reset();

	if( pDSound ) { pDSound->DuplicateSoundBuffer( s.pDSBuffer, &pDSBuffer ); }

	return *this;
}

Sound::~Sound()
{
	if( hEvent ) CloseHandle( hEvent );
	RELEASE( pDSNotify );
	RELEASE( pDSBuffer );
}

void Sound::Reset()
{
	pDSBuffer = NULL;
	pDSNotify = NULL;
	hEvent = NULL;
}

void Sound::CreateDirectSound()
{
	DirectSoundCreate8(
		&DSDEVID_DefaultPlayback,
		&pDSound,
		NULL );

	pDSound->SetCooperativeLevel(
		GetHWnd(),
		DSSCL_PRIORITY );
}

void Sound::ReleaseDirectSound()
{
	RELEASE( pDSound );
}

BOOL Sound::Load( LPCTSTR filename )
{
	FILE *fp;
	if( _tfopen_s( &fp, filename, _T( "rb" ) ) ) {
		CautionMessage( _T( "errorz" ), _T( "ファイルが開けません" ) );
		return FALSE;
	}

	char buf[ 10 ];
	
	fread_s( buf, 4, 1, 4, fp );

	if( memcmp( buf, "RIFF", 4 ) != 0 ) {
		CautionMessage( _T( "errorz" ), _T( "RIFFフォーマットではありません" ) );
		fclose( fp );
		return FALSE;
	}

	fseek( fp, 4, SEEK_CUR );

	fread_s( buf, 8, 1, 8, fp );
	if( memcmp( buf, "WAVEfmt ", 8 ) != 0 ){
		CautionMessage( _T( "errorz" ), _T( "WAVEフォーマットではないか、フォーマット定義がありません" ) );
		fclose( fp );
		return FALSE;
	}

	fseek( fp, 4, SEEK_CUR );

	WAVEFORMATEX wavf;
	fread_s( &wavf, sizeof( WAVEFORMATEX ) - 2, 1, sizeof( WAVEFORMATEX ) - 2, fp );

	ZeroMemory( buf, 10 );
	while( strcmp( "data", buf ) ) {
		buf[ 0 ] = fgetc( fp );
		if( buf[ 0 ] == EOF ) {
			CautionMessage( _T( "errorz" ), _T( "波形定義データが見つかりません" ) );
			fclose( fp );
			return FALSE;
		}
		if( buf[ 0 ] == 'd' ) { fread_s( &buf[ 1 ], 3, 1, 3, fp ); }
	}

	int wsize;
	fread_s( &wsize, sizeof( int ), 1 , sizeof( int ), fp );

	DSBUFFERDESC desc;
	ZeroMemory( &desc, sizeof( DSBUFFERDESC ) );

	desc.dwSize = sizeof( DSBUFFERDESC );
	desc.dwFlags = DSBCAPS_LOCDEFER | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPOSITIONNOTIFY;
	desc.dwBufferBytes = wsize;
	desc.lpwfxFormat = &wavf;

	RELEASE( pDSBuffer );
	HRESULT hr = pDSound->CreateSoundBuffer( &desc, &pDSBuffer, NULL );
	
	DWORD buff_size;
	LPVOID pvAudioPtr;

	pDSBuffer->Lock( 0, 0,
		&pvAudioPtr, &buff_size,
		NULL, NULL,
		DSBLOCK_ENTIREBUFFER );

	fread_s( pvAudioPtr, buff_size, 1, buff_size, fp );

	pDSBuffer->Unlock( pvAudioPtr, buff_size, NULL, NULL );

	fclose( fp );

	return TRUE;
}

struct PlayThreadData
{
	HANDLE hEvent;
	Sound *soundobj;
};

void Sound::Play()
{
	if( pDSBuffer ) {
		pDSBuffer->SetCurrentPosition( 0 );
		pDSBuffer->Play( 0, 0, 0 );

		if( pDSNotify ){
			PlayThreadData *data = _NEW( PlayThreadData );
			data->hEvent = hEvent;
			data->soundobj = this;
			DWORD threadID;
			CreateThread( NULL, 0, Sound::NotifyFunction, data, 0, &threadID );
		}
	}
}

void Sound::Stop()
{
	if( pDSBuffer ) { pDSBuffer->Stop(); }
}

void Sound::SetVolume( LONG volume )
{
	if( pDSBuffer ) { pDSBuffer->SetVolume( volume ); }
}

void Sound::SetPan( LONG lPan )
{
	if( pDSBuffer ) { pDSBuffer->SetPan( lPan ); }
}

void Sound::EnableDeleteByEnd()
{
	if( pDSBuffer == NULL ) { return; }

	HRESULT hr = pDSBuffer->QueryInterface(
		IID_IDirectSoundNotify, ( LPVOID* )&pDSNotify );

	hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	DSBPOSITIONNOTIFY pn;
	pn.dwOffset = DSBPN_OFFSETSTOP;
	pn.hEventNotify = hEvent;

	pDSNotify->SetNotificationPositions( 1, &pn );
}

DWORD Sound::NotifyFunction( LPVOID param )
{
	PlayThreadData *data = ( PlayThreadData* )param;

	WaitForMultipleObjects( 1, &data->hEvent, FALSE, INFINITE );

	_DELETE( data->soundobj );
	_DELETE( data );

	return 0L;
}
