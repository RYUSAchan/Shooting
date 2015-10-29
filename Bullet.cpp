//Bullet.cpp

#include "Bullet.h"

Character* BaseBullet::PlayerPointer = NULL;

BaseBullet::BaseBullet( _TaskList *list )
	:	Character( list )
{}

void BaseBullet::SetPlayerPointer( Character *pointer )
{
	if( pointer == NULL ) { return; }
	PlayerPointer = pointer;
}

void BaseBullet::ResetPlayerPointer()
{
	PlayerPointer = NULL;
}

void* NormalBullet::operator new( size_t i )
{
	return ListObj.Bullet->New( i );
}

void NormalBullet::operator delete( void* p )
{
	ListObj.Bullet->Delete( p );
}

NormalBullet::NormalBullet( _TaskList *list, DrawTexture *tex, float X, float Y, float speed, float angle, float dr )
	:	BaseBullet( list ), texture( tex )
{
	x = X;
	y = Y;
	r = dr;
	Angle = angle;
	Speed = speed;
	Scale = 1.0f;
	Alive = TRUE;
}

void NormalBullet::Uninit()
{}

void NormalBullet::Move()
{
	x += cosf( d2r( Angle - 90.0f ) ) * Speed;
	y += sinf( d2r( Angle - 90.0f ) ) * Speed;
	
	if( x < SCREEN_LEFT  - 16.0f || x > SCREEN_RIGHT + 16.0f || y < SCREEN_TOP - 16.0f || y > SCREEN_BOTTOM + 16.0f )
	{
		Alive = FALSE;
	}
}

void NormalBullet::Draw()
{	
	texture->Draw( x, y, d2r( Angle ) );
}
