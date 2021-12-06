#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CInterrupt : public CGameObject, public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(22.0f, 18.0f);

	void InitAnimations();
	void InitColliders();
public:
	CInterrupt();
	~CInterrupt();

	void Update(DWORD dt);
	void Render();
};