#pragma once
#include "Bullet.h"

class CBallBomb : public CBullet
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(10.0f, 10.0f);
	const int maxBouncingCount = 5;
	const DWORD aliveDuration = 3000;
	int bouncingCount = 0;
	float bouncingPush;
	DWORD initTime;

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

