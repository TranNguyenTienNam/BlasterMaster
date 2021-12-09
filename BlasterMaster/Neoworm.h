#pragma once
#include "Enemy.h"

class CNeoworm : public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(9.0f, 9.0f);

	void InitSprites();
	void InitAnimations();
	void InitColliders();
public:
	CNeoworm();
	~CNeoworm();

	void Update(DWORD dt);
	void Render();
};