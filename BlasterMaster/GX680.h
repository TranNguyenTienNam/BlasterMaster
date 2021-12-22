#pragma once
#include "Enemy.h"

enum class GX680State
{
	FreeMotion,
	DetectedTarget,
};

class CGX680 : public CEnemy
{
protected:
	const Vector2 DEFAULT_SIZE = Vector2(18.0f, 16.0f);
	const float distanceTrigger = 70.0f;
	const float MOVE_SPEED = 0.03f;
	const DWORD changeDirectionDelay = 2000;
	const DWORD shootingDelay = 5000;

	float shootingRate;

	DWORD lastTimeShooting;
	DWORD lastTimeChangeDirection;

	RectF operatingArea;

	GX680State state;

	void InitAnimations();
	void InitColliders();

	void FreeMotion();
	void DetectedTarget();
public:
	CGX680();
	~CGX680();

	void Update(DWORD dt);
	void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};