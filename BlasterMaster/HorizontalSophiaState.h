#pragma once
#include "SophiaState.h"

class HorizontalSophiaState : public ISophiaState
{
public:
	virtual void Motion();
	virtual void Jumping();
	virtual void Shooting();
	virtual void Update(DWORD dt);
	virtual void Render();
};