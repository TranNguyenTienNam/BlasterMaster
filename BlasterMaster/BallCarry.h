#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CBallCarry : public CGameObject, public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(16.0f, 16.0f);

	void InitAnimations();
	void InitColliders();
public:
	CBallCarry();
	~CBallCarry();

	void Update(DWORD dt);
	void Render();
};