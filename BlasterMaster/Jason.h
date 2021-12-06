#pragma once
#include "GameObject.h"
#include "Playable.h"

enum JasonState
{
	JASON_IDLE,
	JASON_MOVING_LEFT,
	JASON_MOVING_RIGHT,
	JASON_JUMPING
};

class CPlayable;

class CJason : public CGameObject, public CPlayable
{
private:
	const float MOVE_SPEED = 0.1f;
	const float MOVE_ACCELERATION = 0.0002f;
	const float JUMP_SPEED = 0.53f;
	const float GRAVITY = -0.0026f;
	const Vector2 IDLE_SIZE = Vector2(8.0f, 16.0f);

	D3DCOLOR tempColor; // temp

	JasonState state;
	LPANIMATION animation;
protected:
	void InitAnimations();
	void InitColliders();
public:
	CJason();

	void SetState(JasonState state);

	void Update(DWORD dt);
	void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};