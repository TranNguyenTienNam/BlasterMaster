#pragma once
#include "Playable.h"

enum class BigJasonState
{
	IDLE_FRONT,
	IDLE_BACK,
	IDLE_LEFT,
	IDLE_RIGHT,

	WALK_FRONT,
	WALK_BACK,
	WALK_LEFT,
	WALK_RIGHT,

	DEAD,
};

enum BigJasonKeySet
{
	MOVE_LEFT = DIK_LEFT,
	MOVE_RIGHT = DIK_RIGHT,
	MOVE_TOP = DIK_UP,
	MOVE_DOWN = DIK_DOWN,
	SHOOTING = DIK_C,
};

class CBigJason : public CPlayable
{
private:
	const float WALK_SPEED = 0.1f;
	const Vector2 IDLE_SIZE = Vector2(20.0f, 12.0f);

	int ny;

	LPANIMATION animation;

	BigJasonState currentState, lastState;

	void InitAnimations();
	void InitColliders();
public:
	CBigJason();

	void SetState(BigJasonState state);

	void Shooting();

	void Update(DWORD dt);
	void Render();
	void OnDead();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

