#pragma once
#include "Bullet.h"
class CJasonBullet : public CBullet
{
private:
	const DWORD aliveDuration = 600;
	const float DEFAULT_SPEED = 0.15f;

	DWORD initTime;
protected:
	void InitSprites();
public:
	CJasonBullet();
	~CJasonBullet();

	virtual void Update(DWORD dt);
	virtual void Render();

	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

