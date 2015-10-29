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

	_Task *Prev, *Next;		//�O��^�X�N�̃|�C���^

	void* operator new( size_t t ) {}		//�f�t�H���g���Z�q�̖���
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

	_Task* ActiveTask;		//�A�N�e�B�u�^�X�N�̐擪�̃_�~�[
	_Task* FreeTask;		//�t���[�^�X�N�̐擪�̃_�~�[

	size_t TaskSize;		//�ő�T�C�Y
	size_t TaskCount;		//�^�X�N��

	_Task* GetTask( size_t i );		//�^�X�N�擾

public:

	_TaskList( size_t size, size_t count );
	~_TaskList();

	void* New( size_t size );
	void Delete( void *p );

};

class _TaskIterator
{

private:

	_TaskList* List;	//�Ώۂ̃��X�g
	_Task* Task;

public:

	_TaskIterator( _TaskList *list );	//�R���X�g���N�^
	BOOL HasNext();						//�������邩
	_Task* Next();						//���̃^�X�N�Ɉړ�
	void Remove();						//���̃^�X�N���폜

};
