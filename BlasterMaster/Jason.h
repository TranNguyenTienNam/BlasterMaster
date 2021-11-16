#pragma once
#include "GameObject.h"
#include "Playable.h"

#define JASON_WALKING_SPEED		0.1f
#define JASON_JUMP_SPEED_Y		0.53f
#define JASON_GRAVITY			0.0026f

enum JasonState
{
	IDLE,
	WALKING_LEFT,
	WALKING_RIGHT,
	JASON_JUMPING
};

class CJason : public CGameObject, public CPlayable
{
protected:
	void InitAnimations();

	JasonState state;
	LPANIMATION animation;

public:
	CJason();
	~CJason();
	void SetState(JasonState state);

	void Update(DWORD dt);
	void Render();
};