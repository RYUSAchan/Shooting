//test.cpp

#include "test.h"
#include <sstream>
#include <locale.h>

TestWorld::TestWorld()
{}

TestWorld::TestWorld( Sequence* ptr )
{
	SetGameObject( ptr );
	tex = dynamic_cast< Texture* >( FindObjectBox( "terrain" ) );
	terrain = _NEW DrawTexture( tex );
	font = dynamic_cast< DxFont* >( FindObjectBox( "MS_UI_Gothic_16" ) );
	x = 320;
	y = 240;
	z = rz = 0.0f;
}

TestWorld::~TestWorld()
{}

void TestWorld::Uninit()
{
	_DELETE( terrain );
}

void TestWorld::update()
{
	GetFrame();
	Input_device->GetKeyState();
	if( Input_device->IsKeyDown( DIK_ESCAPE ) )
	{
		obj->SetNextMode( obj->MODE_END );
		obj->RemoveModeObject();
	} else {

		if( Input_device->IsKeyDown( DIK_UP ) ) { y -= 1.0f; }
		if( Input_device->IsKeyDown( DIK_DOWN ) ) { y += 1.0f; }
		if( Input_device->IsKeyDown( DIK_LEFT ) ) { x -= 1.0f; }
		if( Input_device->IsKeyDown( DIK_RIGHT ) ) { x += 1.0f; }
		if( Input_device->IsKeyDown( DIK_W ) ) { z -= 1.0f; }
		if( Input_device->IsKeyDown( DIK_S ) ) { z += 1.0f; }
		if( Input_device->IsKeyDown( DIK_A ) ) { rz -= 0.01f; }
		if( Input_device->IsKeyDown( DIK_D ) ) { rz += 0.01f; }
			
		//terrain->Draw( 320.0f, 240.0f );

		stringstream ss;
		ss << "( " << x << ", " << y << ", " << z << ", " << rz << " )" << endl;
		TCHAR text[ 32 ];
		
		size_t w = 0;

		mbstowcs_s( &w, text, 50, ss.str().c_str(), _TRUNCATE );

		font->Draw( text, 0, 0 );
		
	}
	SleepFrame();
}
