//Input.h

#pragma once

#include "GameObject.h"

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dxguid.lib" )

#define PP_UP 0x1
#define PP_RIGHT 0x2
#define PP_DOWN 0x4
#define PP_LEFT 0x8

struct enumdata
{
	LPDIRECTINPUT8 pInput;
	LPDIRECTINPUTDEVICE8 *ppPadDevice;
};

class Input : public GameObject			//シングルトン
{
private:
	LPDIRECTINPUT8 pInput;
	LPDIRECTINPUTDEVICE8 pKeyDevice;

	LPDIRECTINPUTDEVICE8 pPadDevice;
	DIJOYSTATE2 paddata, lastpaddata;

	BYTE keydata[ 256 ], lastkeydata[ 256 ];				//入力状態の保持

	Input();
	~Input();

	static Input* mInstance;
	static BOOL CALLBACK EnumJoyPad( const DIDEVICEINSTANCE *pInstance, LPVOID pContext );
	static BOOL CALLBACK EnumObject( LPCDIDEVICEOBJECTINSTANCE pInstance, LPVOID pvRef );

public:
	static Input* Instance();
	void Uninitalize();
	void Initalize();
	
	void GetKeyState();					//キー入力チェック

	BOOL IsKeyDown( int key );
	BOOL IsKeyPressed( int key );
	BOOL IsKeyReleased( int key );

	BYTE GetPovPosition();
	BOOL IsButtonDown( int pos );
	BOOL IsButtonPressed( int pos );
	BOOL IsButtonRelease( int pos );
};