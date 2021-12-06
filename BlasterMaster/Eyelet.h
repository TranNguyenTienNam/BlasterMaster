#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CEyelet : public CGameObject, public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(18.0f, 15.0f);

	void InitAnimations();
	void InitColliders();
public:
	CEyelet();
	~CEyelet();

	void Update(DWORD dt);
	void Render();
};