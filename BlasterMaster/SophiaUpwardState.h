#pragma once
#include "SophiaState.h"

class CSophiaUpwardState : public ISophiaState
{
public:
	virtual void Shooting();
	virtual void Update(DWORD dt, CSophia& sophia, int nx);
	virtual void Render();
};