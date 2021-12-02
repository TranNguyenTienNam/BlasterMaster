#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CBallbot : public CGameObject, public CEnemy
{
private:
	void InitAnimations();
public:
	CBallbot();
	~CBallbot();

	void Update(DWORD dt);
	void Render();
};