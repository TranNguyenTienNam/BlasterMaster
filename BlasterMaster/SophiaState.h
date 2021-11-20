#pragma once
#include "Sophia.h"

class CSophia;

class ISophiaState
{
public:
	virtual void Shooting();
	virtual void UpdateColliders(CSophia& sophia, int nx);
	virtual void Update(DWORD dt, CSophia& sophia, int nx);
	virtual void Render(CSophia& sophia);
};
