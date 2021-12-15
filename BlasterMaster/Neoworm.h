#pragma once
#include "Enemy.h"

class CNeoworm : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(18.0f, 10.0f);
	const float MOVE_SPEED = 0.02f;

	void InitAnimations();
	void InitColliders();
public:
	CNeoworm();
	~CNeoworm();

	void Update(DWORD dt);
	void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};