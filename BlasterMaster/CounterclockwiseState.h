#pragma once
#include "SophiaState.h"

class CCounterclockwiseState : public ISophiaState
{
public:
	ISophiaState* HandleInput(CSophia& sophia);
	void Render(CSophia& sophia);
};