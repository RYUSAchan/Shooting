//Enemy.h

#pragma once

#include "Character.h"
#include "Bullet.h"

class Enemy_1 : public Character
{

private:
	DrawTexture *Enemygraph, *Bulletgraph;

	int frame;

	int count;

	int count_2;
	int count_3;

	Character* playerpointer;

public:
	Enemy_1( _TaskList *list );
	~Enemy_1();

	void Uninit();

	void Move();
	void Draw();

	void* operator new( size_t i );
	void operator delete( void* p );

};