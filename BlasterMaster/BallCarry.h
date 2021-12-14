#pragma once
#include "Enemy.h"

enum class BallCarryState
{
	Sleeping,
	DropingBomb,
	RuningAway,
};

class CBallCarry : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(16.0f, 16.0f);
	const float distanceTrigger = 50.0f;
	const int maxBomb = 4;
	const DWORD dropBombDelay = 1200;

	bool didDroppedBomb = false;
	DWORD lastTimeDropBomb;

	BallCarryState state;

	void InitSprites();
	void InitAnimations();
	void InitColliders();

	void DetectedTarget();
	void DroppingBomb();
	void PrepareRunning();
public:
	CBallCarry();
	~CBallCarry();

	void Update(DWORD dt);
	void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};