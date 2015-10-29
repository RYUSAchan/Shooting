//Tasksys.h

#pragma once

#include "d3d9.h"
#include "d3dx9.h"
#include <assert.h>

class _TaskList;
class _TaskIterator;

class _Task
{

	friend _TaskList;
	friend _TaskIterator;

private:

	_Task *Prev, *Next;		//前後タスクのポインタ

	void* operator new( size_t t ) {}		//デフォルト演算子の無効
	void operator delete( void* p ) {}

public:

	_Task( _TaskList *list );
	virtual ~_Task();

};

class _TaskList
{

	friend _Task;
	friend _TaskIterator;

private:

	char *Buffer;

	_Task* ActiveTask;		//アクティブタスクの先頭のダミー
	_Task* FreeTask;		//フリータスクの先頭のダミー

	size_t TaskSize;		//最大サイズ
	size_t TaskCount;		//タスク数

	_Task* GetTask( size_t i );		//タスク取得

public:

	_TaskList( size_t size, size_t count );
	~_TaskList();

	void* New( size_t size );
	void Delete( void *p );

};

class _TaskIterator
{

private:

	_TaskList* List;	//対象のリスト
	_Task* Task;

public:

	_TaskIterator( _TaskList *list );	//コンストラクタ
	BOOL HasNext();						//次があるか
	_Task* Next();						//次のタスクに移動
	void Remove();						//そのタスクを削除

};
