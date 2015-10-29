//test.h

#pragma once

#include "Mode.h"
#include "Draw.h"

class TestWorld : public Mode
{
private:
	DrawTexture *terrain;
	Texture *tex;
	DxFont *font;
	float x, y, z;
	float rz;

public:
	TestWorld();
	TestWorld( Sequence* ptr );
	~TestWorld();

	void update();
	void Uninit();
};