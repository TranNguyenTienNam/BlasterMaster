#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CInterrupt : public CGameObject, public CEnemy
{
private:
	void InitAnimations();
public:
	CInterrupt();
	~CInterrupt();

	void Update(DWORD dt);
	void Render();
};