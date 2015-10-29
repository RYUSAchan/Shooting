//Player.cpp

#include "Player.h"

void* Player::operator new( size_t i )
{
	return ListObj.Player->New( i );
}

void Player::operator delete( void* p )
{
	ListObj.Player->Delete( p );
}

Player::Player( _TaskList *list )
	:	Character( list )
{
	Playergraph = _NEW DrawTexture( dynamic_cast< Texture* >( FindObjectBox( "Graph_Player" ) ) );
	Hitgraph = _NEW DrawTexture( dynamic_cast< Texture* >( FindObjectBox( "Graph_Player_Hit" ) ) );
	explosion = _NEW DrawTexture( dynamic_cast< Texture* >( FindObjectBox( "Graph_Explosion" ) ) );
	explosion->SetSpriteSize( 48, 48 );
	Playergraph->SetSpriteSize( 48, 48 );
	x = 224.0f;
	y = SCREEN_BOTTOM - 28;
	r = 4;
	explode = FALSE;
	Alive = TRUE;
	Scale = 1.0f;
	count = 0;
	AppendItemBox( "Stage_1_Player", this );
}

Player::~Player()
{}

void Player::Move()
{
	float move_s = 3.0f,
		  move_a = move_s / sqrtf( 2.0f );

	if( count == 16 )
	{
		Alive = FALSE;
	}

	if( Input_device->IsKeyDown( DIK_LSHIFT ) || Input_device->IsKeyDown( DIK_RSHIFT ) )
	{
		slow = TRUE;
		move_s /= 2.0f;
		move_a /= 2.0f;
	} else {
		slow = FALSE;
	}

	BOOL left = Input_device->IsKeyDown( DIK_LEFT );  
	BOOL right = Input_device->IsKeyDown( DIK_RIGHT );  
	BOOL up = Input_device->IsKeyDown( DIK_UP );  
	BOOL down = Input_device->IsKeyDown( DIK_DOWN );  
    
	if( up & left ) {
		x -= move_a;
		y -= move_a;
	} else if( up & right ) {
		x += move_a;
		y -= move_a;
	} else if( down & left ) {
		x -= move_a;
		y += move_a;
	} else if( down & right ) {
		x += move_a;
		y += move_a;
	} else if( up ) {
		y -= move_s;
	} else if( down ) {
		y += move_s;
	} else if( left ) {
		x -= move_s;
	} else if ( right ) {
		x += move_s;
	}

	if( x < SCREEN_LEFT + 14.0f ) { x = SCREEN_LEFT + 14.0f; }
	if( x > SCREEN_RIGHT - 14.0f ) { x = SCREEN_RIGHT - 14.0f; }
	if( y < SCREEN_TOP + 24.0f ) { y = SCREEN_TOP + 24.0f; }
	if( y > SCREEN_BOTTOM - 16.0f ) { y = SCREEN_BOTTOM - 16.0f; }

	for( _TaskIterator i( ListObj.Enemy ); i.HasNext(); )
	{
		Character* chara = static_cast< Character* >( i.Next() );
		if( IsHit( chara ) ) { explode = TRUE; }
	}
	for( _TaskIterator i( ListObj.Bullet ); i.HasNext(); )
	{
		Character* chara = static_cast< Character* >( i.Next() );
		if( IsHit( chara ) ) { explode = TRUE; }
	}
}

void Player::Draw()
{
	if( explode ) {
		explosion->Draw( x, y );
		count++;
		explosion->SetFrame( count );
	} else {
		Playergraph->Draw( x, y - 4.0f );
		if( slow ) { Hitgraph->Draw( x, y ); }
	}
}

void Player::Uninit()
{
	_DELETE( Playergraph );
	_DELETE( Hitgraph );
	_DELETE( explosion );
	RemoveItemBox( "Stage_1_Player" );
}

