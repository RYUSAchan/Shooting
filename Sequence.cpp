//Sequence.cpp

#include "Sequence.h"
#include "Input.h"
#include "Sound.h"
#include "BGMusic.h"
#include "Draw.h"
#include "ModeDef.h"
#include "test.h"

void Sequence::SetPointer( HWND hWnd )
{
	SetWindowLong( hWnd, GWL_USERDATA, ( LONG )this );
}

LRESULT CALLBACK Sequence::CallProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	Sequence* thisPtr = ( Sequence* )GetWindowLong( hWnd, GWL_USERDATA );

	if( !thisPtr )
	{
		if( msg == WM_CREATE ){ thisPtr = ( Sequence* )( ( LPCREATESTRUCT )lParam )->lpCreateParams; }

		if( thisPtr )
		{
			thisPtr->SetPointer( hWnd );
		}
	}

	if( thisPtr )
	{
		return thisPtr->MainProc( hWnd, msg, wParam, lParam );
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

LRESULT Sequence::MainProc( HWND hWnd, UINT msg, WPARAM wPalam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
		break;

	default:
		return DefWindowProc( hWnd, msg, wPalam, lParam );
	}
}

BOOL Sequence::Initialize( HINSTANCE hInst, int nCmdShow )
{
	SetHInstance( hInst );
	Obj = NULL;
	CoInitialize( NULL );

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = ( HBRUSH )GetStockObject( WHITE_BRUSH );
	wc.hCursor = LoadCursor( hInst, IDC_ARROW );
	wc.hIcon = NULL;
	wc.hInstance = hInst;
	wc.lpfnWndProc = CallProc;
	wc.lpszClassName = _T( "Game" );
	wc.lpszMenuName = NULL;
	wc.style = NULL;

	if( RegisterClass( &wc ) == NULL ) {
		return FALSE;
	}

	SetHWnd( CreateWindow(
		wc.lpszClassName,
		_T( "Game2015" ),
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		NULL, NULL, hInst, ( LPVOID )this ) );

	pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( pD3D == NULL ) {
		CautionMessage( _T( "errorz" ), _T( "DirectXDevice‚Ì‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½" ) ); 
		return FALSE;
	}

	D3DDISPLAYMODE d3ddm;
	if( FAILED( pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
	{
		CautionMessage( _T( "errorz" ), _T( "DirectX3DDevice‚Ì‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½" ) );
		return FALSE;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.BackBufferWidth = 640;
	d3dpp.BackBufferHeight = 480;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if( FAILED( pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetHWnd(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &pD3Ddevice ) ) )
	{
		CautionMessage( _T( "errorz" ), _T( "3DDeviceObject‚Ì‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½" ) );
		return FALSE;
	}

	pD3Ddevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pD3Ddevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	pD3Ddevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	if( FAILED( D3DXCreateSprite( pD3Ddevice, &pSprite ) ) )
	{
		CautionMessage( _T( "errorz" ), _T( "SpriteObject‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B" ) );
		return FALSE;
	}

	Input_device = Input::Instance();
	Input_device->Initalize();
	Sound::CreateDirectSound();

	NextMode = MODE_UNKNOWN;
	objarrive = TRUE;
	Obj = _NEW Load( this );

	ShowWindow( GetHWnd(), nCmdShow );

	return TRUE;
}

void Sequence::MainLoop()
{
	MSG msg;

	while( TRUE ){
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
				if( msg.message == WM_QUIT ) break;
				TranslateMessage( &msg );
				DispatchMessage( &msg );
		} else {
			pD3Ddevice->Clear( 0, NULL, D3DCLEAR_TARGET,
				D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

			pD3Ddevice->BeginScene();

			Obj->update();
			if( !objarrive )
			{
				Obj->Uninit();
				_DELETE( Obj );
				switch( NextMode )
				{
				case MODE_TITLE:
					Obj = _NEW Title( this );
					break;
				case MODE_STAGE_1:
					Obj = _NEW Stage_1( this );
					break;
				case MODE_TEST:
					Obj = _NEW TestWorld( this );
					break;
				case MODE_END:
				case MODE_UNKNOWN:
				default:
					DestroyWindow( GetHWnd() );
				}

				objarrive = TRUE;
				gamemode = NextMode;
				NextMode = MODE_UNKNOWN;
			}

			pD3Ddevice->EndScene();
			pD3Ddevice->Present( NULL, NULL, NULL, NULL );
		}
	}
}

void Sequence::Uninitialize()
{
	if( Obj ) 
	{ 
		Obj->Uninit();
		_DELETE( Obj );
	}
	Input_device->Uninitalize();
	RELEASE( pSprite );
	RELEASE( pD3Ddevice );
	RELEASE( pD3D );
	Sound::ReleaseDirectSound();
	CoUninitialize();
	ResetObjectBox();
}

void Sequence::SetNextMode( Seq next )
{
	NextMode = next;
}

void Sequence::RemoveModeObject()
{
	objarrive = FALSE;
}
