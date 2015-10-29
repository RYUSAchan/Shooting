//GameObject.h

#pragma once

#include "d3d9.h"
#include "d3dx9.h"
#include <assert.h>
#include <tchar.h>
#include <cstdio>
#include <hash_map>
#include <string>
#include <fstream>
#include <sstream>
#include "Tasksys.h"

using namespace std;
using namespace stdext;

#define SCREEN_W 384
#define SCREEN_H 448
#define SCREEN_W_H 192
#define SCREEN_H_H 224

#define SCREEN_LEFT 32
#define SCREEN_RIGHT 415
#define SCREEN_TOP 16
#define SCREEN_BOTTOM 463

#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>
#include <crtdbg.h>

#ifndef NDEBUG

#define _NEW ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )

#else

#define _NEW new

#endif

#define _DELETE( p ) { delete p; p = NULL; }

#define RELEASE( x ) { if( x ) x->Release(); }

inline float d2r( float angle ) { return D3DX_PI / 180 * angle; }

class Input;

class ListObject
{

public:
	_TaskList *Enemy;
	_TaskList *Bullet;
	_TaskList *Bullet_Spare;
	_TaskList *Player;
	_TaskList *Object;

	ListObject();
	~ListObject();
};


class GameObject
{
private:
	static HWND hWnd;
	static HINSTANCE hInstance;

	static DWORD mStartTime;
	static int mCount;
	static float mfps;

	static hash_map< string, GameObject* > ObjBox, ItemBox;
	
public:
	static ListObject ListObj;

	static const HWND GetHWnd() { return hWnd; }
	static const HINSTANCE GetHInstace() { return hInstance; }
	
	static void SetHWnd( HWND hw ) { hWnd = hw; }
	static void SetHInstance( HINSTANCE hi ) { hInstance = hi; }

	void CautionMessage( LPTSTR title, LPTSTR errmsg );

	GameObject() {}
	virtual ~GameObject() {}
	
	static void AppendObjectBox( LPCSTR name, GameObject* object );
	static void RemoveObjectBox( LPCSTR name );
	static void RemoveObjectBox_NoDelete( LPCSTR name );
	static void ResetObjectBox();
	static void ResetObjectBox_NoDelete();
	static GameObject* FindObjectBox( LPCSTR name );

	static void AppendItemBox( LPCSTR name, GameObject* object );
	static void RemoveItemBox( LPCSTR name );
	static void ResetItemBox();
	static GameObject* FindItemBox( LPCSTR name );

protected:
	static LPDIRECT3D9 pD3D;
	static LPDIRECT3DDEVICE9 pD3Ddevice;
	static LPD3DXSPRITE pSprite;

	static Input* Input_device;

	static void GetFrame();
	static void SleepFrame();
	static const float GetFPS() { return mfps; }
};
