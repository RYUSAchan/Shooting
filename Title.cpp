//Title.cpp

#include "Title.h"

Title::Title()
	:	Mode(), sstate( MOVE_1 )
{}

Title::Title( Sequence* ptr )
	:	sstate( MOVE_1 ), Mode( ptr )
{
	DrawFps = dynamic_cast< DxFont* >( FindObjectBox( "MS_UI_Gothic_16" ) );
	title = dynamic_cast< DxFont* >( FindObjectBox( "MS_MINCHO_52" ) );
	column = dynamic_cast< DxFont* >( FindObjectBox( "MS_UI_Gothic_28" ) );
	column_italic = dynamic_cast< DxFont* >( FindObjectBox( "HGS_GYOSYOTAI_32_Italic" ) );
	Cursor = _NEW DrawTexture( dynamic_cast< Texture* >( FindObjectBox( "Cursor" ) ) );
	Cursor->SetSpriteRect( 0, 0, 24, 24 );
	Gandhi[ 0 ] = _NEW DrawTexture( dynamic_cast< Texture* >( FindObjectBox( "Graph_Gandhi_II_1" ) ) );
	Gandhi[ 1 ] = _NEW DrawTexture( dynamic_cast< Texture* >( FindObjectBox( "Graph_Gandhi_II_2" ) ) );
	Gandhi[ 2 ] = _NEW DrawTexture( dynamic_cast< Texture* >( FindObjectBox( "Graph_Gandhi_II_3" ) ) );
	Gandhi[ 0 ]->SetSpriteRect( 0, 0, 480, 360 );
	Gandhi[ 1 ]->SetSpriteRect( 0, 0, 480, 360 );
	Gandhi[ 2 ]->SetSpriteRect( 0, 0, 480, 360 );

	Count = 0;
	frame = 0;
	cursorpos = 0;
	x = y = 0;
}

Title::~Title()
{}

void Title::Uninit()
{
	_DELETE( Cursor );
	_DELETE( Gandhi[ 0 ] );
	_DELETE( Gandhi[ 1 ] );
	_DELETE( Gandhi[ 2 ] );
}

void Title::update()
{
	GetFrame();
	//stringstream ss;
	
	Input_device->GetKeyState();
	if( Input_device->IsKeyDown( DIK_ESCAPE ) )
	{
		obj->SetNextMode( Sequence::MODE_END );
		obj->RemoveModeObject();
	}
	/*if( Input_device->IsKeyDown( DIK_F5 ) )
	{
		ifstream ifs( "cfg.txt" );
		ss.clear();
		char c;
		while( ( c = ifs.get() ) != ',' )
		{
			ss << c;
		}
		ss >> x;
		ss.clear();
		while ( ( c = ifs.get() ) != EOF )
		{
			ss << c;
		}
		ss >> y;
	}*/

	//ss.clear();
	
	switch( sstate ) {
	case MOVE_1:
		column->Draw( _T( "î ÇØ ÇÈ" ), 260, 280 );
		column->Draw( _T( "êÊêlÇÃÇ®åæót" ), 260, 340 );
		column->Draw( _T( "í˙ Çﬂ ÇÈ" ), 260, 400 );
		switch( cursorpos ) {
		case 0:
			Cursor->Draw( 240.0f, 293.0f );
			break;
		case 1:
			Cursor->Draw( 240.0f, 353.0f );
			break;
		case 2:
			Cursor->Draw( 240.0f, 413.0f );
			break;
		}
		PROGRAM_TITLE_DRAW;
		if( Input_device->IsKeyPressed( DIK_Z ) ) {
			if( cursorpos == 0 )
			{
				sstate = MOVE_2;
				Count = 0;
				break;
			} else if( cursorpos == 1 )
			{
				sstate = PREDECESSOR;
				Count = 0;
				cursorpos = 0;
				break;
			} else if ( cursorpos == 2 )
			{
				obj->SetNextMode( Sequence::MODE_END );
				obj->RemoveModeObject();
				break;
			}
		}
		if( Input_device->IsKeyPressed( DIK_DOWN ) && !Input_device->IsKeyDown( DIK_UP ) ) {
			cursorpos++;
			if( cursorpos > 2 ) { cursorpos = 2; }
		} else if( Input_device->IsKeyPressed( DIK_UP ) && !Input_device->IsKeyDown( DIK_DOWN ) ) {
			cursorpos--;
			if( cursorpos < 0 ) { cursorpos = 0; }
		}
		break;
	case MOVE_2:
		column->Draw( _T( "î ÇØ ÇÈ" ), 260 - Count * 5, 280 );
		Count++;
		if( Count == 30 )
		{
			sstate = MOVE_3;
			Count = 0;
			cursorpos = 1;
		}
		PROGRAM_TITLE_DRAW;
		break;
	case MOVE_3:
		column->Draw( _T( "î ÇØ ÇÈ" ), 110, 280 );
		PROGRAM_TITLE_DRAW;
		if( cursorpos == 1 )
		{
			column->Draw( _T( "Stage 1" ), 260, 280 );
			column->Draw( _T( "Stage 2" ), 260, 320 );
			column->Draw( _T( "Stage 3" ), 260, 360 );
			Cursor->Draw( 240, 293.0f );
		} else if( cursorpos == MAX_COLUMN )
		{
			TCHAR buf[16];
			swprintf_s( buf, _T( "Stage %d" ), MAX_COLUMN - 2 );
			column->Draw( buf, 260, 280 );
			swprintf_s( buf, _T( "Stage %d" ), MAX_COLUMN - 1 );
			column->Draw( buf, 260, 320 );
			swprintf_s( buf, _T( "Stage %d" ), MAX_COLUMN );
			column->Draw( buf, 260, 360 );
			Cursor->Draw( 240.0f, 373.0f );
		} else {
			TCHAR buf[16];
			swprintf_s( buf, _T( "Stage %d" ), cursorpos - 1 );
			column->Draw( buf, 260, 280 );
			swprintf_s( buf, _T( "Stage %d" ), cursorpos );
			column->Draw( buf, 260, 320 );
			swprintf_s( buf, _T( "Stage %d" ), cursorpos + 1 );
			column->Draw( buf, 260, 360 );
			Cursor->Draw( 240.0f, 333.0f );
		}
		if( Input_device->IsKeyPressed( DIK_Z ) )
		{
			NextStage( cursorpos );
			break;
		}
		if( Input_device->IsKeyPressed( DIK_X ) )
		{
			sstate = MOVE_4;
			Count = 30;
			cursorpos = 0;
			break;
		}
		if( Input_device->IsKeyPressed( DIK_DOWN ) && !Input_device->IsKeyDown( DIK_UP ) ) {
			if( cursorpos != MAX_COLUMN ) { cursorpos++; }
		} else if( Input_device->IsKeyPressed( DIK_UP ) && !Input_device->IsKeyDown( DIK_DOWN ) ) {
			if( cursorpos != 1 ) { cursorpos--; }
		}
		break;
	case MOVE_4:
		column->Draw( _T( "î ÇØ ÇÈ" ), 260 - Count * 5, 280 );
		PROGRAM_TITLE_DRAW;
		Count--;
		if( Count == 0 )
		{
			sstate = MOVE_1;
			Count = 0;
			cursorpos = 0;
		}
		break;
	case PREDECESSOR:
		Gandhi[ cursorpos ]->Draw( 320.0f, 160.0f, 0.8f, 0.8f );
		if( frame >= 30 ) {
			if( frame < 94 ) {
				RECT rect = { 70, 330, 70, 330 };
				column_italic->Draw( _T( "\"îÒñ\óÕÇÕêlä‘Ç…ó^Ç¶ÇÁÇÍÇΩ" ), -1, &rect, DT_NOCLIP |DT_SINGLELINE,
					0x00FFFFFF | ( ( BYTE )( Count * 4 ) << 24 ) );
				Count++;
			} else if( frame == 94 ) {
				Count = 0;
				column_italic->Draw( _T( "\"îÒñ\óÕÇÕêlä‘Ç…ó^Ç¶ÇÁÇÍÇΩ" ), 70, 330 );
			} else {
				column_italic->Draw( _T( "\"îÒñ\óÕÇÕêlä‘Ç…ó^Ç¶ÇÁÇÍÇΩ" ), 70, 330 );
			}
		}
		if( frame >= 150 ) {
			if( frame < 214 ) {
				RECT rect = { 380, 330, 380, 330 };
				column_italic->Draw( _T( "ç≈ëÂÇÃïêäÌÇ≈Ç†ÇË" ), -1, &rect, DT_NOCLIP |DT_SINGLELINE,
					0x00FFFFFF | ( ( BYTE )( Count * 4 ) << 24 ) );
				Count++;
			} else if( frame == 214 ) {
				Count = 0;
				column_italic->Draw( _T( "ç≈ëÂÇÃïêäÌÇ≈Ç†ÇË" ), 380, 330 );
			} else {
				column_italic->Draw( _T( "ç≈ëÂÇÃïêäÌÇ≈Ç†ÇË" ), 380, 330 );
			}
		}
		if( frame >= 270 ) {
			if( frame < 334 ) {
				RECT rect = { 60, 360, 60, 360 };
				column_italic->Draw( _T( "êlä‘Ç™î≠ñæÇµÇΩç≈ã≠ÇÃïêäÌÇÊÇËÇ‡" ), -1, &rect, DT_NOCLIP |DT_SINGLELINE,
					0x00FFFFFF | ( ( BYTE )( Count * 4 ) << 24 ) );
				Count++;
			} else if( frame == 334 ) {
				Count = 0;
				column_italic->Draw( _T( "êlä‘Ç™î≠ñæÇµÇΩç≈ã≠ÇÃïêäÌÇÊÇËÇ‡" ), 60, 360 );
			} else {
				column_italic->Draw( _T( "êlä‘Ç™î≠ñæÇµÇΩç≈ã≠ÇÃïêäÌÇÊÇËÇ‡" ), 60, 360 );
			}
		}
		if( frame >= 390 ) {
			if( frame < 454 ) {
				RECT rect = { 427, 360, 427, 360 };
				column_italic->Draw( _T( "ã≠Ç¢óÕÇéùÇ¬ \"" ), -1, &rect, DT_NOCLIP |DT_SINGLELINE,
					0x00FFFFFF | ( ( BYTE )( Count * 4 ) << 24 ) );
				Count++;
			} else if( frame == 454 ) {
				Count = 0;
				column_italic->Draw( _T( "ã≠Ç¢óÕÇéùÇ¬ \"" ), 427, 360 );
			} else {
				column_italic->Draw( _T( "ã≠Ç¢óÕÇéùÇ¬ \"" ), 427, 360 );
			}
		}
		if( frame >= 510 ) {
			if( frame < 574 ) {
				RECT rect = { 10, 410, 10, 410 };
				column_italic->Draw( _T( "ê¢äEÇ≈ç≈Ç‡ñ\óÕÇ©ÇÁâìÇ¢íj" ), -1, &rect, DT_NOCLIP |DT_SINGLELINE,
					0x00FFFFFF | ( ( BYTE )( Count * 4 ) << 24 ) );
				Count++;
			} else if( frame == 574 ) {
				Count = 0;
				column_italic->Draw( _T( "ê¢äEÇ≈ç≈Ç‡ñ\óÕÇ©ÇÁâìÇ¢íj" ), 10, 410 );
			} else {
				column_italic->Draw( _T( "ê¢äEÇ≈ç≈Ç‡ñ\óÕÇ©ÇÁâìÇ¢íj" ), 10, 410 );
			}
		}
		if( frame >= 630 ) {
			if( frame < 694 ) {
				RECT rect = { 60, 440, 60, 440 };
				column_italic->Draw( _T( "Mohandas Karamchand Gandhi (1869-1948)" ), -1, &rect, DT_NOCLIP |DT_SINGLELINE,
					0x00FFFFFF | ( ( BYTE )( Count * 4 ) << 24 ) );
				Count++;
			} else if( frame == 694 ) {
				Count = 0;
				column_italic->Draw( _T( "Mohandas Karamchand Gandhi (1869-1948)" ), 60, 440 );
			} else {
				column_italic->Draw( _T( "Mohandas Karamchand Gandhi (1869-1948)" ), 60, 440 );
			}
		}
		/*TCHAR t[ 32 ];
		wsprintf( t, _T( "( %d, %d )" ), x, y );
		DrawFps->Draw( t, 0, 0 );*/
		
		if( frame < 700 && Input_device->IsKeyPressed( DIK_Z ) )
		{
			if( frame < 93 ) {
				frame = 93;
			} else if( frame < 213 ) {
				frame = 213;
			} else if( frame < 333 ) {
				frame = 333;
			} else if( frame < 453 ) {
				frame = 453;
			} else if( frame < 573 ) {
				frame = 573;
			} else if( frame < 693 ) {
				frame = 693;
			}
		}
		if( Input_device->IsKeyPressed( DIK_X ) )
		{
			sstate = MOVE_1;
			Count = 0;
			cursorpos = 0;
		}
		if( Input_device->IsKeyPressed( DIK_RIGHT ) && !Input_device->IsKeyDown( DIK_LEFT ) ) {
			cursorpos++;
			if( cursorpos > 2 ) { cursorpos = 0; }
		} else if( Input_device->IsKeyPressed( DIK_LEFT ) && !Input_device->IsKeyDown( DIK_RIGHT ) ) {
			cursorpos--;
			if( cursorpos < 0 ) { cursorpos = 2; }
		}
		/*if( Input_device->IsKeyDown( DIK_RIGHT ) && !Input_device->IsKeyDown( DIK_LEFT ) ) {
			x++;
		} else if( Input_device->IsKeyDown( DIK_LEFT ) && !Input_device->IsKeyDown( DIK_RIGHT ) ) {
			x--;
		}
		if( Input_device->IsKeyDown( DIK_DOWN ) && !Input_device->IsKeyDown( DIK_UP ) ) {
			y++;
		} else if( Input_device->IsKeyDown( DIK_UP ) && !Input_device->IsKeyDown( DIK_DOWN ) ) {
			y--;
		}*/
		if( frame < 700 ){ frame++; }
	}

	SleepFrame();
}

void Title::NextStage( int pos )
{
	switch( pos )
	{
	case 1:
		obj->SetNextMode( obj->MODE_STAGE_1 );
		break;
	case 2:
		obj->SetNextMode( obj->MODE_STAGE_2 );
		break;
	case 3:
		obj->SetNextMode( obj->MODE_STAGE_3 );
		break;
	case 4:
		obj->SetNextMode( obj->MODE_STAGE_4 );
		break;
	case 5:
		obj->SetNextMode( obj->MODE_STAGE_5 );
		break;
	}
	obj->RemoveModeObject();
}