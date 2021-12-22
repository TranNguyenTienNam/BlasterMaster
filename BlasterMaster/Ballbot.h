#pragma once
#include "Enemy.h"

enum class BallbotState
{
	Sleeping,
	DetectedTarget,
};

class CBallbot : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(18.0f, 18.0f);
	const float MOVE_SPEED = 0.2f;
	const float distanceTrigger = 90.0f;

	BallbotState state;

	void InitAnimations();
	void InitColliders();
public:
	CBallbot();
	~CBallbot();

	void SetState(BallbotState state);
	void Sleeping();
	void DetectedTarget();
	void Update(DWORD dt);
	void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};