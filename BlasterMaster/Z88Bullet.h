#pragma once
#include "Bullet.h"
class CZ88Bullet : public CBullet
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(8.0f, 16.0f);

	void InitSprites();
	void InitColliders();
public:
	CZ88Bullet();
	~CZ88Bullet();

	virtual void Update(DWORD dt);
	virtual void Render();

	void OnOverlapped(CCollider2D* selfCollider, CGameObject* object);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

