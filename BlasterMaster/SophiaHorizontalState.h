#pragma once
#include "SophiaState.h"

#define BOX_SOPHIA_IDLE Vector2(24.0f, 17.0f)
#define OFFSET_SOPHIA_IDLE Vector2(0.0f, 4.5f)

class CSophiaHorizontalState : public ISophiaState
{
protected:
	const DWORD turningDelay = 100;
	DWORD lastTimeTurning;
	bool isTurning;
public:
	CSophiaHorizontalState();
	void Turning();
	bool IsTurning() { return this->isTurning; }

	virtual void Shooting();
	virtual void UpdateColliders();
	virtual void Update(DWORD dt);
	virtual void Render();
};