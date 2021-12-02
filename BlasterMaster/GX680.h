#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CGX680 : public CGameObject, public CEnemy
{
private:
	void InitAnimations();
public:
	CGX680();
	~CGX680();

	void Update(DWORD dt);
	void Render();
};