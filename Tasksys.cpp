//Tasksys.cpp

#include "Tasksys.h"

_Task* _TaskList::GetTask( size_t i )
{
	return ( _Task* )( Buffer + TaskSize * i );
}

_TaskList::_TaskList( size_t size, size_t count )
	:	TaskSize( size ), TaskCount( count )
{
	Buffer = new char[ TaskSize * ( TaskCount + 2 ) ];

	ActiveTask = GetTask( 0 );
	ActiveTask->Prev = ActiveTask->Next = ActiveTask;

	FreeTask = GetTask( 1 );
	for ( size_t i = 1; i < TaskCount + 2; i++ )
	{
		GetTask( i )->Prev = NULL;
		GetTask( i )->Next = GetTask( i + 1 );
	}
	GetTask( TaskCount + 1 )->Next = FreeTask;
}

_TaskList::~_TaskList()
{
	delete [] Buffer;
}

void* _TaskList::New( size_t size )
{
	assert( size <= TaskSize );
	
	if( FreeTask->Next == FreeTask ) { return NULL; }

	_Task* task = FreeTask->Next;
	FreeTask->Next = task->Next;
	return task;
}

void _TaskList::Delete( void* p )
{
	_Task *task = ( _Task* )p;

	assert( task->Prev != NULL );
	
	task->Prev = NULL;
	task->Next = FreeTask->Next;
	FreeTask->Next = task;
}

_Task::_Task( _TaskList* list )
{
	Prev = list->ActiveTask->Prev;
	Next = list->ActiveTask;
	Prev->Next = Next->Prev = this;
}

_Task::~_Task()
{
	Prev->Next = Next;
	Next->Prev = Prev;
}

_TaskIterator::_TaskIterator( _TaskList* list ) 
	:	List( list ), Task( list->ActiveTask )
{}

BOOL _TaskIterator::HasNext()
{
	return ( Task->Next != List->ActiveTask );
}

_Task* _TaskIterator::Next()
{
	return Task = Task->Next;
}

void _TaskIterator::Remove()
{
	Task = Task->Prev;
	delete Task->Next;
}
