//WinMain.cpp

#include "Sequence.h"

int APIENTRY _tWinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpszCmdLine, int CmdShow )
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	Sequence* game = _NEW Sequence;
	if( !game->Initialize( hInstance, CmdShow ) ) { return 0; }
	game->MainLoop();
	game->Uninitialize();
	_DELETE( game );

	return 0;
}