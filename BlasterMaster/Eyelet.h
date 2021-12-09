#pragma once
#include "Enemy.h"

class CEyelet : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(18.0f, 15.0f);
	const float angularAmplitude = 30.0f;
	const float angularFrequency = 0.2f;
	float angular = 0;

	void InitAnimations();
	void InitColliders();
public:
	CEyelet();
	~CEyelet();

	void Update(DWORD dt);
	void Render();
};