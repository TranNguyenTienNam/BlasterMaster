#pragma once
#include "GameObject.h"
#include "Enemy.h"

class CStuka : public CGameObject, public CEnemy
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(18.0f, 18.0f);

	void InitAnimations();
	void InitColliders();
public:
	CStuka();
	~CStuka();

	void Update(DWORD dt);
	void Render();
};