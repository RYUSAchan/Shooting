//Player.h

#pragma once

#include "Character.h"

class Player : public Character
{
private:
	DrawTexture *Playergraph, *Hitgraph, *explosion;

	BOOL slow;
	BOOL explode;

	int count;
	
public:
	Player( _TaskList *list );
	~Player();

	void Uninit();

	void Move();
	void Draw();

	void* operator new( size_t i );
	void operator delete( void* p );
};