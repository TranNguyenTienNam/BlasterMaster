#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CEyelet : public CGameObject, public CEnemy
{
private:
	void InitAnimations();
public:
	CEyelet();
	~CEyelet();

	void Update(DWORD dt);
	void Render();
};