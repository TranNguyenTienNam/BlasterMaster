#pragma once
#include "Sophia.h"

class CSophia;

class ISophiaState
{
public:
	virtual ISophiaState* HandleInput(CSophia& sophia);
	virtual void Motion();
	virtual void Jumping();
	virtual void Shooting();
	virtual void Update(DWORD dt);
	virtual void Render(CSophia& sophia);
};
