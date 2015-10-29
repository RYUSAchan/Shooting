//Stage.h

#pragma once

#include "Mode.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

class Stage_1 : public Mode
{
public:
	enum STATE {
		MOVE_1,
		MOVE_2,
		MOVE_3,
		MOVE_4
	};

private:
	DrawTexture *DrawBase;
	STATE sstate;

	DxFont *font_1, *font_2, *font_3;

	int frame, count;

public:
	Stage_1();
	Stage_1( Sequence *ptr );
	~Stage_1();

	void update();
	void Uninit();
};