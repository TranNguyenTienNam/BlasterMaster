#pragma once
#include "Enemy.h"

class CStuka : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(18.0f, 18.0f);
	const float MOVE_SPEED = 0.02f;

	void InitAnimations();
	void InitColliders();
public:
	CStuka();
	~CStuka();

	void SetDirection(int nx) override;
	void Update(DWORD dt);
	void Render();
};