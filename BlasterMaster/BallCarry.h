#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CBallCarry : public CGameObject, public CEnemy
{
private:
	void InitAnimations();
public:
	CBallCarry();
	~CBallCarry();

	void Update(DWORD dt);
	void Render();
};