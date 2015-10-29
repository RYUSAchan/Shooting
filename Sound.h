//Sound.h

#pragma once

#include "GameObject.h"

#include <dsound.h>
#pragma comment( lib, "dsound.lib" )
#pragma comment( lib, "dxguid.lib" )

class Sound : public GameObject
{
private:
	
	static LPDIRECTSOUND8 pDSound;

	LPDIRECTSOUNDBUFFER pDSBuffer;

	LPDIRECTSOUNDNOTIFY pDSNotify;
	HANDLE hEvent;
	static DWORD WINAPI NotifyFunction( LPVOID param );

	void Reset();

public:
	
	static void CreateDirectSound();
	static void ReleaseDirectSound();

	Sound();
	Sound( LPCTSTR filename );
	Sound( const Sound& );
	~Sound();

	BOOL Load( LPCTSTR filename );

	void Play();
	void Stop();
	void SetVolume( LONG volume );
	void SetPan( LONG lPan );

	Sound& operator=( const Sound& );

	void EnableDeleteByEnd();
	
};
