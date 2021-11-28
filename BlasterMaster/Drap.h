#pragma once
#include "GameObject.h"

#define DRAP_WALKING_SPEED		0.15f

#define DRAP_SIZE Vector2(16.0f, 16.0f)

class CDrap : public CGameObject
{
protected:
	void InitAnimations();

public:
	CDrap();
	~CDrap();
	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};