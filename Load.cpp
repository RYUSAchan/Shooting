//Load.cpp

#include "Load.h"
#include "Draw.h"
#include <process.h>

BOOL Load::complete = FALSE;

Load::Load( Sequence* ptr )
{
	SetGameObject( ptr );

	tex = _NEW Texture( _T( "data/nowloading.png" ) );
	drawtex = new DrawTexture( tex );
	drawtex->SetSpriteSize( 240, 48 );

	complete = FALSE;

	_beginthread( this->LoadThread, 0, NULL );

}

Load::~Load()
{}

void Load::update()
{
	GetFrame();
	if( !complete )
	{
		brightness = ( ( brightness + 253 ) % 255 );

		drawtex->SetCenterPosition( CP_LEFT | CP_TOP );

		drawtex->Draw( 427.0f, 444.0f, brightness );
	} else {
		//obj->SetNextMode( obj->MODE_TITLE );
		obj->SetNextMode( obj->MODE_TEST );
		obj->RemoveModeObject();
	}
	SleepFrame();
}

void Load::Uninit()
{
	_DELETE( drawtex );
	_DELETE( tex );
}

void Load::LoadThread( void *data )
{
	AppendObjectBox( "MS_UI_Gothic_16", _NEW DxFont( 16 ) );
	AppendObjectBox( "MS_UI_Gothic_28", _NEW DxFont( 28 ) );
	AppendObjectBox( "HGS_GYOSYOTAI_32_Italic", _NEW DxFont( 32, _T( "HGS_çsèëëÃ" ), TRUE ) );  
	AppendObjectBox( "MS_MINCHO_52", _NEW DxFont( 52, _T( "ÇlÇr ñæí©" ), FALSE ) );

	AppendObjectBox( "Cursor", _NEW Texture( _T( "data/cursor.png" ) ) );
	AppendObjectBox( "Graph_Player", new Texture( _T( "data/Player.png" ) ) );
	AppendObjectBox( "Graph_Enemy", new Texture( _T( "data/Duke3D.png" ) ) );
	AppendObjectBox( "Graph_Player_Hit", _NEW Texture( _T( "data/hit.png" ) ) );
	AppendObjectBox( "Graph_Base",  _NEW Texture( _T( "data/Base.png" ) ) );
	AppendObjectBox( "Graph_Bullet" , new Texture( _T( "data/tshoot.png" ) ) );
	AppendObjectBox( "Graph_Explosion", _NEW Texture( _T( "data/explosion.png" ) ) );
	AppendObjectBox( "Graph_Gandhi_II_1", _NEW Texture( _T( "data/gandhi_ii_1.png" ) ) );
	AppendObjectBox( "Graph_Gandhi_II_2", _NEW Texture( _T( "data/gandhi_ii_2.png" ) ) );
	AppendObjectBox( "Graph_Gandhi_II_3", _NEW Texture( _T( "data/gandhi_ii_3.png" ) ) );
	AppendObjectBox( "terrain", _NEW Texture( _T( "data/terrain.png" ) ) );

	complete = TRUE;
}