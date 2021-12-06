#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CLaserGuard : public CGameObject, public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(17.0f, 18.0f);

	void InitAnimations();
	void InitColliders();
public:
	CLaserGuard();
	~CLaserGuard();

	void Update(DWORD dt);
	void Render();
};