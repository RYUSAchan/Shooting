//Character.cpp

#include "Character.h"

Character::Character( _TaskList *list )
	: _Task( list )
{}

Character::Character( _TaskList *list, float dx, float dy, float dangle, float dr )
	:	_Task( list ), x( dx ), y( dy ), Angle( dangle ), Alive( TRUE ), r( dr )
{}

Character::~Character()
{}

BOOL Character::IsHit( Character* chara )
{
	float dx = chara->GetX() - x,
		  dy = chara->GetY() - y,
		  hit = chara->GetR() + r;

	return dx * dx + dy * dy < hit * hit;
}
