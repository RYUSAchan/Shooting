//Title.h

#pragma once

#include "Mode.h"
#include "Draw.h"

#define MAX_COLUMN 5

#define PROGRAM_TITLE_DRAW title->Draw( _T( "Vs Java" ), 220, 120 )

class Title : public Mode
{
private:
	enum StageState
	{
		MOVE_1,
		MOVE_2,
		MOVE_3,
		MOVE_4,
		
		PREDECESSOR
	};

	StageState sstate;

	DxFont *DrawFps, *title, *column, *column_italic;
	DrawTexture *Cursor;
	DrawTexture* Gandhi[3];

	int x, y;

	int cursorpos;

	int Count, frame;

	void NextStage( int pos );

public:
	Title();
	Title( Sequence* ptr );
	~Title();

	void update();
	void Uninit();
};