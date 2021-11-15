#pragma once
#include "SophiaState.h"

class CSophiaUpward45State : public ISophiaState
{
public:
	virtual void Shooting();
	virtual void Update(DWORD dt, CSophia& sophia, int nx);
	virtual void Render();
};