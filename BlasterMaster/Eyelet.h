#pragma once
#include "Enemy.h"

class CEyelet : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(18.0f, 15.0f);
	const float angularAmplitude = 30.0f;
	const float angularFrequency = 0.2f;
	float angular = 0;
	float axisY;

	void InitAnimations();
	void InitColliders();
public:
	CEyelet();
	~CEyelet();

	void SetInitAxisY(float y) { this->axisY = y; }
	void Update(DWORD dt);
	void Render();
};