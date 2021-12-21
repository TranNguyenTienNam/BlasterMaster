#pragma once
#include "Enemy.h"

#define DRAP_WALKING_SPEED		0.15f

class CDrap : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(16.0f, 16.0f);
	const float MOVE_SPEED = 0.2f;
	int monitorSide;
	float rangeTrigger = 3.0f;

	void InitAnimations();
	void InitColliders();
public:
	CDrap();
	~CDrap();
	void Update(DWORD dt);
	void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};