//Mode.cpp

#include "Mode.h"

Mode::Mode()
	:	obj( NULL )
{}

Mode::Mode( Sequence* ptr )
{
	SetGameObject( ptr );
}

void Mode::SetGameObject( Sequence* ptr )
{
	if( ptr == NULL ) { return; }
	obj = ptr;
}