//Character.h

#pragma once

#include "GameObject.h"
#include "Draw.h"
#include "Input.h"

class Character : public GameObject, public _Task
{

protected:

	float x, y;

	float r;

	float Angle;

	float Scale;

	BOOL Alive;

public:

	const float GetX() { return x; }
	const float GetY() { return y; }
	const float GetAngle() { return Angle; }
	const float GetScale() { return Scale; }
	const float GetR() { return r; }
	const BOOL GetAlive() { return Alive; }

	Character( _TaskList *list );
	Character( _TaskList *list, float dx, float dy, float dangle, float dr );
	virtual ~Character();
	virtual void Uninit() {}

	virtual void Move() {}
	virtual void Draw() {}

	BOOL IsHit( Character* chara );
	
};