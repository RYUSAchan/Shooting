//Load.h

#pragma once

#include "Mode.h"
#include "Draw.h"

class Load : public Mode
{
private:

	int brightness;
	Texture* tex;
	DrawTexture* drawtex;

public:

	Load() {}
	Load( Sequence* ptr );
	~Load();

	static void LoadThread( void *data );

protected:
	static BOOL complete;
	void update();
	void Uninit();
};