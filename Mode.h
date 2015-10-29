//Mode.h

#pragma once

#include "Sequence.h"
#include "Input.h"

class Mode : public GameObject
{
protected:
	Sequence *obj;

public:
	void SetGameObject( Sequence* ptr );
	
	Mode();
	Mode( Sequence* ptr );
	virtual ~Mode() {}

	virtual void update() {}
	virtual void Uninit() {}
};