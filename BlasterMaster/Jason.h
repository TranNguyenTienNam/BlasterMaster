#pragma once
#include "Playable.h"

enum class JasonState
{
	IDLE,
	MOVING_LEFT,
	MOVING_RIGHT,
	JUMPING,
	DEAD,
};

class CPlayable;

class CJason : public CPlayable
{
private:
	const float WALK_SPEED = 0.1f;
	const float WALK_ACCELERATION = 0.0002f;
	const float JUMP_SPEED = 0.53f;
	const float GRAVITY = -0.0026f;
	const float CROUCH_SPEED = 0.02f;
	const Vector2 IDLE_SIZE = Vector2(8.0f, 16.0f);

	D3DCOLOR damagedColor[4] =
	{
		D3DCOLOR_ARGB(255,148,247,207),
		D3DCOLOR_ARGB(128,124,124,124),
		D3DCOLOR_ARGB(255,247,164,143),
		D3DCOLOR_ARGB(128,124,124,124),
	};

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
	void OnDead();

	void OnOverlapped(CCollider2D* selfCollider, CGameObject* object);
	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};