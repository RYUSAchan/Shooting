//Sequence.h

#pragma once

#include "GameObject.h"

class Mode;

class Sequence : public GameObject
{
public:

	enum Seq {
		MODE_LOAD,
		MODE_TITLE,
		MODE_END,
		MODE_STAGE_1,
		MODE_STAGE_2,
		MODE_STAGE_3,
		MODE_STAGE_4,
		MODE_STAGE_5,

		MODE_TEST,
		MODE_UNKNOWN
	};

private:
	Seq gamemode;

	Mode *Obj;
	BOOL objarrive;

	Seq NextMode;

	void SequenceUpdate();

public:
	Sequence() {}
	~Sequence() {}

	void SetPointer( HWND hWnd );

	static LRESULT CALLBACK CallProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static LRESULT MainProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	const Seq GetGameMode() { return gamemode; }

	BOOL Initialize( HINSTANCE hInst, int nCmdShow );
	void Uninitialize();

	void MainLoop();

	void SetNextMode( Seq next );
	void RemoveModeObject();
};