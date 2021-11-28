#pragma once
#include "SophiaState.h"

#define BOX_SOPHIA_UPWARD Vector2(24.0f, 32.0f)
#define OFFSET_SOPHIA_UPWARD Vector2(-3.0f, 12.0f)

class CSophiaUpwardState : public ISophiaState
{
public:
	virtual void Shooting();
	virtual void UpdateColliders();
	virtual void Update(DWORD dt);
	virtual void Render();
};