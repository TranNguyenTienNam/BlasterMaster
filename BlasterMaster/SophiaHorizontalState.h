#pragma once
#include "SophiaState.h"

#define BOX_SOPHIA_IDLE Vector2(24.0f, 17.0f)
#define OFFSET_SOPHIA_IDLE Vector2(0.0f, 4.5f)

class CSophiaHorizontalState : public ISophiaState
{
public:
	virtual void Shooting();
	virtual void UpdateColliders();
	virtual void Update(DWORD dt);
	virtual void Render();
};