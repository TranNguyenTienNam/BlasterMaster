#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CNeoworm : public CGameObject, public CEnemy
{
private:
	void InitAnimations();
public:
	CNeoworm();
	~CNeoworm();

	void Update(DWORD dt);
	void Render();
};