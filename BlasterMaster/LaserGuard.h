#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CLaserGuard : public CGameObject, public CEnemy
{
private:
	void InitAnimations();
public:
	CLaserGuard();
	~CLaserGuard();

	void Update(DWORD dt);
	void Render();
};