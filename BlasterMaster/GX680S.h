#pragma once
#include "GX680.h"
#include "Enemy.h"

class CGX680S : public CGameObject, public CEnemy
{
private:
	void InitAnimations();
public:
	CGX680S();
	~CGX680S();

	void Update(DWORD dt);
	void Render();
};