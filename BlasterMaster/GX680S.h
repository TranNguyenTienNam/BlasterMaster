#pragma once
#include "GX680.h"
#include "Enemy.h"

class CGX680S : public CGameObject, public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(18.0f, 16.0f);

	void InitAnimations();
	void InitColliders();
public:
	CGX680S();
	~CGX680S();

	void Update(DWORD dt);
	void Render();
};