//Enemy.cpp

#include "Enemy.h"

void* Enemy_1::operator new( size_t i )
{
	return ListObj.Enemy->New( i );
}

void Enemy_1::operator delete( void* p )
{
	ListObj.Enemy->Delete( p );
}

Enemy_1::Enemy_1( _TaskList *list )
	:	Character( list )
{
	Enemygraph = _NEW DrawTexture( dynamic_cast< Texture* >( FindObjectBox( "Graph_Enemy" ) ) );
	Enemygraph->SetSpriteSize( 48, 48 );
	Enemygraph->SetCenterPosition( CP_MIDDLECENTER );
	Bulletgraph = new DrawTexture( dynamic_cast< Texture* >( FindObjectBox( "Graph_Bullet" ) ) );
	Bulletgraph->SetSpriteSize( 12, 16 );
	x = 224.0f;
	y = SCREEN_TOP + 64;

	Alive = TRUE;
	Scale = 1.0f;
	frame = 0;
	count = 5;
	count_2 = 0;

	playerpointer = static_cast< Character* >( FindItemBox( "Stage_1_Player" ) );
	BaseBullet::SetPlayerPointer( playerpointer );
}

Enemy_1::~Enemy_1()
{
}

void Enemy_1::Draw()
{
	Enemygraph->Draw( x, y );
}

void Enemy_1::Move()
{
	if( !( frame % 4 ) ) {
		new NormalBullet( ListObj.Bullet, Bulletgraph, x, y, 3.0f, count * 15, 2.0f );
		new NormalBullet( ListObj.Bullet, Bulletgraph, x, y, 3.0f, -( count * 15 ), 2.0f );
		new NormalBullet( ListObj.Bullet, Bulletgraph, x, y, 3.0f, 120.0f + ( count_2 * 5 ) , 2.0f );
		count = ( count + 1 ) % 24;
		if( count_2 == 0 ) { count_3 = 1; }
		if( count_2 == 24 ) { count_3 = -1; }
		count_2 += count_3;
	}
	frame = ( frame + 1 ) % 480;
	if( !( frame % 60 ) ) {
		float span = 360.0f / 48;
		float angle = 0.0f;

		for( int i = 1; i <= 48; i++ ) {
			new NormalBullet( ListObj.Bullet, Bulletgraph, x + ( SCREEN_LEFT / 2 ), y, 3.0f, angle, 2.0f );
			new NormalBullet( ListObj.Bullet, Bulletgraph, x - ( SCREEN_LEFT / 2 ), y, 3.0f, angle, 2.0f );
			angle += span;
		}
	}

}

void Enemy_1::Uninit()
{
	_DELETE( Enemygraph );
	_DELETE( Bulletgraph );
}
