#pragma once
#include "GameObject.h"

#define DRAP_WALKING_SPEED		0.15f

#define DRAP_STATE_WALKING_RIGHT	100
#define DRAP_STATE_WALKING_LEFT		200
#define DRAP_STATE_WALKING_TOP		300
#define DRAP_STATE_WALKING_DOWN		400
#define DRAP_STATE_IDLE				500

#define DRAP_WIDTH 16.0f
#define DRAP_HEIGHT 16.0f

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