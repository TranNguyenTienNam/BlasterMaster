#pragma once
#include "GameObject.h"

#define PLAYER_WALKING_SPEED		0.15f

#define PLAYER_STATE_WALKING_RIGHT	100
#define PLAYER_STATE_WALKING_LEFT		200
#define PLAYER_STATE_WALKING_TOP		300
#define PLAYER_STATE_WALKING_DOWN		400
#define PLAYER_STATE_IDLE				500

class CPlayable : public CGameObject
{
protected:
	void InitAnimations();

public:
	CPlayable();
	~CPlayable();
	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};