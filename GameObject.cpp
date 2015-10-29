//GameObject.cpp

#include "GameObject.h"

HWND GameObject::hWnd = NULL;
HINSTANCE GameObject::hInstance = NULL;

LPDIRECT3D9 GameObject::pD3D = NULL;
LPDIRECT3DDEVICE9 GameObject::pD3Ddevice = NULL;
LPD3DXSPRITE GameObject::pSprite = NULL;

Input* GameObject::Input_device = NULL;
ListObject GameObject::ListObj;

hash_map< string, GameObject* > GameObject::ObjBox;
hash_map< string, GameObject* > GameObject::ItemBox;

DWORD GameObject::mStartTime = 0;
int GameObject::mCount = 0;
float GameObject::mfps = 0.0f;

ListObject::ListObject()
	:	Enemy( NULL ), Bullet( NULL ), Bullet_Spare( NULL ), Player( NULL ), Object( NULL )
{}

ListObject::~ListObject()
{
	_DELETE( Enemy );
	_DELETE( Bullet );
	_DELETE( Bullet_Spare );
	_DELETE( Player );
	_DELETE( Object );
}

void GameObject::CautionMessage( LPTSTR title, LPTSTR message )
{
	MessageBox( GetHWnd(), message, title, MB_OK | MB_ICONWARNING );
}

void GameObject::GetFrame()
{
	if( mCount == 0 ) {
		mStartTime = timeGetTime();
	}
	if( mCount == 60 ) {
		DWORD t = timeGetTime();
		mfps = 1000.0f / ( ( t - mStartTime ) / 60.0f );
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
}

void GameObject::SleepFrame()
{
	int tooktime = timeGetTime() - mStartTime;
	int waittime = mCount * 1000 / 60 - tooktime;
	/*TCHAR t[32];
	wsprintf( t, _T( "%d\n" ), waittime );
	OutputDebugString( t );*/
	if( waittime > 0 ) {
		Sleep( waittime );
	}
}

void GameObject::AppendObjectBox( LPCSTR name, GameObject* object )
{
	ObjBox.insert( make_pair( name, object ) );
}

void GameObject::AppendItemBox( LPCSTR name, GameObject* object )
{
	ItemBox.insert( make_pair( name, object ) );
}

void GameObject::RemoveObjectBox( LPCSTR name )
{
	hash_map< string, GameObject* >::iterator i = ObjBox.find( name );
	if( i == ObjBox.end() ) { return; }

	_DELETE( i->second );
	ObjBox.erase( i );
}

void GameObject::RemoveObjectBox_NoDelete( LPCSTR name )
{
	ObjBox.erase( name );
}

void GameObject::RemoveItemBox( LPCSTR name )
{
	ItemBox.erase( name );
}

void GameObject::ResetObjectBox()
{
	hash_map< string, GameObject* >::iterator i;
	for( i = ObjBox.begin(); i != ObjBox.end(); i++ )
	{
		_DELETE( i->second );
	}
	ObjBox.clear();
}

void GameObject::ResetObjectBox_NoDelete()
{
	ObjBox.clear();
}

void GameObject::ResetItemBox()
{
	ItemBox.clear();
}

GameObject* GameObject::FindObjectBox( LPCSTR name )
{
	hash_map< string, GameObject* >::iterator i = ObjBox.find( name );
	
	if( i == ObjBox.end() ) { return NULL; }

	return i->second;
}

GameObject* GameObject::FindItemBox( LPCSTR name )
{
	hash_map< string, GameObject* >::iterator i = ItemBox.find( name );
	
	if( i == ItemBox.end() ) { return NULL; }

	return i->second;
}
