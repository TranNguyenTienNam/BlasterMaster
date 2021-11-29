#pragma once
#include "SophiaState.h"

class CSophiaIdleState : public ISophiaState
{
public:
	void Update(DWORD dt);
	void Render();
};