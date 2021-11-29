#pragma once
#include "SophiaState.h"

class CSophiaMoveRightState : public ISophiaState
{
public:
	void Update(DWORD dt);
	void Render();
};