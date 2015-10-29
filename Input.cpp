//Input.cpp

#include "Input.h"

Input* Input::mInstance = NULL;

Input* Input::Instance()
{
	assert( !mInstance );
	mInstance = _NEW Input;
	return mInstance;
}

Input::Input()
	:	pInput( NULL ), pKeyDevice( NULL ), pPadDevice( NULL )
{}

Input::~Input()
{
	RELEASE( pKeyDevice );
	RELEASE( pInput );
}

void Input::Initalize()
{
	if( pInput == NULL ) {
		HRESULT hr = DirectInput8Create(
			GetHInstace(),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			( LPVOID* )&pInput,
			NULL );

		if( FAILED( hr ) ) {
			CautionMessage( _T( "errorz" ), _T( "DirectInput‚Ì‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½" ) );
			return;
		}

		hr = pInput->CreateDevice(
			GUID_SysKeyboard,
			&pKeyDevice,
			NULL );

		if( FAILED( hr ) ) {
			CautionMessage( _T( "errorz" ), _T( "DirectInputDevice‚Ì‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½" ) );
			return;
		}

		pKeyDevice->SetDataFormat( &c_dfDIKeyboard );
		pKeyDevice->SetCooperativeLevel( GetHWnd(),
			DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );

		ZeroMemory( keydata, sizeof( BYTE ) * 256 );
		ZeroMemory( lastkeydata, sizeof( BYTE ) * 256 );

		enumdata ed;
		ed.pInput = pInput;
		ed.ppPadDevice = &pPadDevice;

		pInput->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			EnumJoyPad,
			&ed,
			DIEDFL_ATTACHEDONLY );

		if( pPadDevice ) {

			pPadDevice->EnumObjects( EnumObject, pPadDevice, DIDFT_AXIS );

			pPadDevice->SetCooperativeLevel( GetHWnd(),
				DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );

			hr = pPadDevice->SetDataFormat( &c_dfDIJoystick2 );
			if( FAILED( hr ) ) { RELEASE( pPadDevice ); }
		}
	}
}

void Input::Uninitalize() {
	if( mInstance ) {
		_DELETE( mInstance );
	}
}

void Input::GetKeyState()
{
	if( pKeyDevice ) {
		pKeyDevice->Acquire();

		memcpy_s( lastkeydata, 256, keydata, 256 );
		pKeyDevice->GetDeviceState( 256, keydata );
	}

	if( pPadDevice ) {
		pPadDevice->Poll();

		pPadDevice->Acquire();

		memcpy_s( &lastpaddata, sizeof( DIJOYSTATE2 ), &paddata, sizeof( DIJOYSTATE2 ) );
		pPadDevice->GetDeviceState( sizeof( DIJOYSTATE2 ), &paddata );
	}
}

BOOL Input::IsKeyDown( int key )
{
	if( pKeyDevice == NULL ) { return FALSE; }

	return ( keydata[ key ] & 0x80 ) ? TRUE : FALSE;
}

BOOL Input::IsKeyPressed( int key )
{
	if( pKeyDevice == NULL ) { return FALSE; }
	return ( ( keydata[ key ] & 0x80 ) && !( lastkeydata[ key ] & 0x80 ) ) ? TRUE : FALSE;
}

BOOL Input::IsKeyReleased( int key )
{
	if( pKeyDevice == NULL ) { return FALSE; }
	return ( !( keydata[ key ] & 0x80 ) && ( lastkeydata[ key ] & 0x80 ) ) ? TRUE : FALSE;
}

BOOL CALLBACK Input::EnumJoyPad( const DIDEVICEINSTANCE *pInstance, LPVOID pContext )
{
	enumdata *ed = ( enumdata* )pContext;

	HRESULT hr;
	hr = ed->pInput->CreateDevice( pInstance->guidInstance, ed->ppPadDevice, NULL );
	if( FAILED( hr ) ) { return DIENUM_CONTINUE; }

	return DIENUM_STOP;
}

BOOL CALLBACK Input::EnumObject( LPCDIDEVICEOBJECTINSTANCE pInstance, LPVOID pvRef )
{
	DIPROPRANGE range;
	range.diph.dwSize = sizeof( DIPROPRANGE );
	range.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	range.diph.dwObj = pInstance->dwType;
	range.diph.dwHow = DIPH_BYID;

	range.lMin = -10;
	range.lMax = +10;

	LPDIRECTINPUTDEVICE8 pInputDev = ( LPDIRECTINPUTDEVICE8 )pvRef;
	pInputDev->SetProperty( DIPROP_RANGE, &range.diph );

	return DIENUM_CONTINUE;
}

BYTE Input::GetPovPosition()
{
	switch( paddata.rgdwPOV[ 0 ] ) {
	case 0:
		return PP_UP;
	case 4500:
		return PP_UP | PP_RIGHT;
	case 9000:
		return PP_RIGHT;
	case 13500:
		return PP_RIGHT | PP_DOWN;
	case 18000:
		return PP_DOWN;
	case 22500:
		return PP_DOWN | PP_LEFT;
	case 27000:
		return PP_LEFT;
	case 31500:
		return PP_LEFT | PP_UP;
	}

	BYTE result = 0x0;

	if( paddata.lX > 5 ) { result |= PP_RIGHT; }
	else if( paddata.lX < -5 ) result |= PP_LEFT;

	if( paddata.lY > 5 ) { result |= PP_DOWN; }
	else if( paddata.lY < -5 ) result |= PP_UP;

	return result;
}

BOOL Input::IsButtonDown( int pos )
{
	if( pPadDevice == NULL ) { return FALSE; }
	return paddata.rgbButtons[ pos ];
}

BOOL Input::IsButtonPressed( int pos )
{
	if( pPadDevice == NULL ) { return FALSE; }
	return ( paddata.rgbButtons[ pos ] && !lastpaddata.rgbButtons[ pos ] ) ? TRUE : FALSE;
}

BOOL Input::IsButtonRelease( int pos )
{
	if( pPadDevice == NULL ) { return FALSE; }
	return ( !paddata.rgbButtons[ pos ] && lastpaddata.rgbButtons[ pos ] ) ? TRUE : FALSE;
}
