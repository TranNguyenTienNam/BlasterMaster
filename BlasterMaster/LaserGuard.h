#pragma once
#include "Enemy.h"

class CLaserGuard : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(17.0f, 18.0f);
	const float MOVE_SPEED = 0.04f;

	void InitAnimations();
	void InitColliders();
public:
	CLaserGuard();
	~CLaserGuard();

	void Update(DWORD dt);
	void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};