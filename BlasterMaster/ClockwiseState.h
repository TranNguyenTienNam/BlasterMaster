#pragma once
#include "SophiaState.h"

class CClockwiseState : public ISophiaState
{
public:
	ISophiaState* HandleInput(CSophia& sophia);
	void Render(CSophia& sophia);
};