#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CStuka : public CGameObject, public CEnemy
{
private:
	void InitAnimations();
public:
	CStuka();
	~CStuka();

	void Update(DWORD dt);
	void Render();
};