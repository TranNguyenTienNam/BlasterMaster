#pragma once
#include "SophiaState.h"

class CWheelIdleState : public ISophiaState
{
public:
	ISophiaState* HandleInput(CSophia& sophia);
	void Render(CSophia& sophia);
};