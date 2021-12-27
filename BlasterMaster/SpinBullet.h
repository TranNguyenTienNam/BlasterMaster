#pragma once
#include "Bullet.h"
class CSpinBullet : public CBullet
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(8.0f, 8.0f);
	const DWORD spinSpeed = 20;
	DWORD lastTimeSpin;

	void InitSprites();
	void InitColliders();
public:
	CSpinBullet();
	~CSpinBullet();

	virtual void Update(DWORD dt);
	virtual void Render();

	void OnOverlapped(CCollider2D* selfCollider, CGameObject* object);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

