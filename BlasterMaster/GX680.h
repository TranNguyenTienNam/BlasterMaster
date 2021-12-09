#pragma once
#include "Enemy.h"

class CGX680 : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(18.0f, 16.0f);

	void InitAnimations();
	void InitColliders();
public:
	CGX680();
	~CGX680();

	void Update(DWORD dt);
	void Render();
};