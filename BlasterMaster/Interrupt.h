#pragma once
#include "Enemy.h"

enum class InterruptState
{
	Sleeping,
	Openning,
};

class CInterrupt : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(22.0f, 18.0f);
	const float distanceTrigger = 96.0f;
	const float rangeTrigger = 10.0f;

	LPSPRITE sprite;

	InterruptState state;

	void InitAnimations();
	void InitColliders();

	void DetectedTarger();
	void DroppingNeowormLarva();
public:
	CInterrupt();
	~CInterrupt();

	void SetState(InterruptState state);

	void Update(DWORD dt);
	void Render();
};