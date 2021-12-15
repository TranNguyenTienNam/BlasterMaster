#pragma once
#include "Bullet.h"
class CNeowormLarva : public CBullet
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(9.0f, 9.0f);
	bool hasHatched = false;

	void InitSprites();
	void InitColliders();

	void Explosion();
public:
	CNeowormLarva();
	~CNeowormLarva();

	virtual void Update(DWORD dt);
	virtual void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

