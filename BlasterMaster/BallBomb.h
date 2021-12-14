#pragma once
#include "GameObject.h"

class CBallBomb : public CGameObject
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(10.0f, 10.0f);
	const int maxBouncingCount = 5;
	int bouncingCount = 0;
	float bouncingPush;

	void InitSprites();
	void InitColliders();

	void Explosion();
public:
	CBallBomb();
	~CBallBomb();

	virtual void Update(DWORD dt);
	virtual void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

