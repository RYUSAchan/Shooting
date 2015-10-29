//BGMusic.h

#pragma once

#include "GameObject.h"
#include <dshow.h>
#pragma comment ( lib, "strmiids.lib" )
#pragma comment ( lib, "winmm.lib" )

class BGMusic : public GameObject
{
private:
	IGraphBuilder *pBuilder;
	IMediaControl *pMediaCtrl;
	IMediaSeeking *pMediaSeeking;
	IBasicAudio *pBasicAudio;

	BOOL bLoop;
	BOOL bFading;

	float time_s, time_e, vol_s, delta;

	void InitializeBGM();
	void Release();

public:
	BGMusic();
	BGMusic( LPCTSTR filename );
	~BGMusic();
	
	void Load( LPCTSTR filename );
	void Play( BOOL loop );
	void Stop();

	void SetVolume( LONG volume );
	void Fade( DWORD endtime, LONG volume );

	void Exec();
};

