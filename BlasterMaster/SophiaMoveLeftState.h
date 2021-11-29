#pragma once
#include "SophiaState.h"

class CSophiaMoveLeftState : public ISophiaState
{
public:
	void Update(DWORD dt);
	void Render();
};