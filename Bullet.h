//Bullet.h

#pragma once

#include "Character.h"

class BaseBullet : public Character
{
protected:
	static Character* PlayerPointer;

public:
	BaseBullet( _TaskList *list );
	virtual ~BaseBullet() {}

	static void SetPlayerPointer( Character* pointer );
	static void ResetPlayerPointer();

};

class NormalBullet : public BaseBullet
{

private:
	
	DrawTexture *texture;
	float Speed;

public:

	NormalBullet( _TaskList *list, DrawTexture *tex, float X, float Y, float speed, float angle, float dr );
	~NormalBullet(){}

	void Uninit();

	void Move();
	void Draw();

	void* operator new( size_t i );
	void operator delete( void* p );

};
