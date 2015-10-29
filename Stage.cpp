//Stage.cpp

#include "Stage.h"

Stage_1::Stage_1()
{}

Stage_1::Stage_1( Sequence* ptr )
{
	SetGameObject( ptr );
	DrawBase = _NEW DrawTexture( dynamic_cast< Texture* >( FindObjectBox( "Graph_Base" ) ) );
	font_1 = dynamic_cast< DxFont* >( FindObjectBox( "MS_MINCHO_52" ) );
	font_2 = dynamic_cast< DxFont* >( FindObjectBox( "MS_UI_Gothic_16" ) );
	font_3 = dynamic_cast< DxFont* >( FindObjectBox( "MS_UI_Gothic_28" ) );
	DrawBase->SetSpriteSize( 640, 480 );
	DrawBase->SetCenterPosition( CP_LEFT | CP_TOP );
	ListObj.Player = _NEW _TaskList( sizeof( Player ), 4 );
	ListObj.Enemy = _NEW _TaskList( sizeof( Enemy_1 ), 4 );
	ListObj.Bullet = _NEW _TaskList( sizeof( NormalBullet ), 512 );
	count = 90;
	frame = 0;
	new Player( ListObj.Player );
	new Enemy_1( ListObj.Enemy );
	sstate = MOVE_1;
}

Stage_1::~Stage_1()
{}

void Stage_1::Uninit()
{
	for( _TaskIterator i( ListObj.Enemy ); i.HasNext(); )
	{
		Character* chara = static_cast< Character* >( i.Next() );
		chara->Uninit();
		i.Remove();
	}
	for( _TaskIterator i( ListObj.Player ); i.HasNext(); )
	{
		Character* chara = static_cast< Character* >( i.Next() );
		chara->Uninit();
		i.Remove();
	}

	_DELETE( DrawBase );
	_DELETE( ListObj.Enemy );
	_DELETE( ListObj.Player );
	_DELETE( ListObj.Bullet );
}

void Stage_1::update()
{
	GetFrame();
	Input_device->GetKeyState();
	if( Input_device->IsKeyDown( DIK_ESCAPE ) )
	{
		obj->SetNextMode( obj->MODE_END );
		obj->RemoveModeObject();
	}else {
		switch( sstate ) {
		case MOVE_1:
			for( _TaskIterator i( ListObj.Enemy ); i.HasNext(); )
			{
				Character* chara = static_cast< Character* >( i.Next() );
				chara->Draw();
			}
			for( _TaskIterator i( ListObj.Player ); i.HasNext(); )
			{
				Character* chara = static_cast< Character* >( i.Next() );
				chara->Draw();
			}
			DrawBase->Draw( 0.0f, 0.0f );
			font_1->Draw( _T( "避けろ！" ), 130, 140 );
			font_2->Draw( _T( "Zキーで始まる" ), 180, 220 );
			if( Input_device->IsKeyPressed( DIK_Z ) )
			{
				sstate = MOVE_2;
			}
			break;
		case MOVE_2:
			for( _TaskIterator i( ListObj.Enemy ); i.HasNext(); )
			{
				Character* chara = static_cast< Character* >( i.Next() );
				chara->Move();
			}
			for( _TaskIterator i( ListObj.Bullet ); i.HasNext(); )
			{
				Character* chara = static_cast< Character* >( i.Next() );
				chara->Move();
				if( !chara->GetAlive() ) {
					chara->Uninit();
					i.Remove();
				}
			}
			for( _TaskIterator i( ListObj.Player ); i.HasNext(); )
			{
				Character* chara = static_cast< Character* >( i.Next() );
				chara->Move();
				if( !chara->GetAlive() ) {
					chara->Uninit();
					i.Remove();
					sstate = MOVE_3;
				}
			}
			for( _TaskIterator i( ListObj.Enemy ); i.HasNext(); )
			{
				Character* chara = static_cast< Character* >( i.Next() );
				chara->Draw();
			}
			for( _TaskIterator i( ListObj.Player ); i.HasNext(); )
			{
				Character* chara = static_cast< Character* >( i.Next() );
				chara->Draw();
			}
			for( _TaskIterator i( ListObj.Bullet ); i.HasNext(); )
			{
				Character* chara = static_cast< Character* >( i.Next() );
				chara->Draw();
			}

			DrawBase->Draw( 0.0f, 0.0f );
			TCHAR buf[10];
			swprintf_s( buf, _T( "%d" ), count );
			RECT rect;
			rect = { 500, 205, 500, 205 };
			font_3->Draw( buf, -1, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000 );
			rect = { 465, 170, 465, 170 };
			font_3->Draw( _T( "残り時間" ), -1, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000 );

			frame = ( frame + 1 ) % 60; 
			if( frame == 0 ) { count--; }
			if( count == 0 ) 
			{
				sstate = MOVE_4;
			}
			break;
		case MOVE_3:
			for( _TaskIterator i( ListObj.Enemy ); i.HasNext(); )
			{
				Character* chara = static_cast< Character* >( i.Next() );
				chara->Draw();
			}
			DrawBase->Draw( 0.0f, 0.0f );
			font_1->Draw( _T( "GAME OVER" ), 105, 140 );
			font_2->Draw( _T( "Zキーで戻る" ), 180, 220 );
			if( Input_device->IsKeyPressed( DIK_Z ) )
			{
				obj->SetNextMode( obj->MODE_TITLE );
				obj->RemoveModeObject();
			}
			break;
		case MOVE_4:
			for( _TaskIterator i( ListObj.Player ); i.HasNext(); )
			{
				Character* chara = static_cast< Character* >( i.Next() );
				chara->Draw();
			}
			DrawBase->Draw( 0.0f, 0.0f );
			font_1->Draw( _T( "GAME CLEAR" ), 100, 140 );
			font_2->Draw( _T( "Zキーで戻る" ), 180, 220 );
			if( Input_device->IsKeyPressed( DIK_Z ) )
			{
				obj->SetNextMode( obj->MODE_TITLE );
				obj->RemoveModeObject();
			}
			break;
		}
	}
	SleepFrame();
}
