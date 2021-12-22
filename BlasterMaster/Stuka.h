#pragma once
#include "Enemy.h"

enum class StukaState
{
	FreeMoving,
	DetectedTarget,
};

class CStuka : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(18.0f, 18.0f);
	const float MOVE_SPEED = 0.04f;
	const float distanceTrigger = 80.0f;

	StukaState state;

	void InitAnimations();
	void InitColliders();
public:
	CStuka();
	~CStuka();

	void SetDirection(int nx) override;

	void FreeMoving();
	void DetectedTarget();
	void Update(DWORD dt);
	void Render();
};