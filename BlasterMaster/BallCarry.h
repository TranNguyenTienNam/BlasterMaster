#pragma once
#include "Enemy.h"

class CBallCarry : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(16.0f, 16.0f);

	void InitAnimations();
	void InitColliders();
public:
	CBallCarry();
	~CBallCarry();

	void Update(DWORD dt);
	void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};