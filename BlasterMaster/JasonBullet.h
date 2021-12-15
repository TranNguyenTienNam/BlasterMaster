#pragma once
#include "Bullet.h"
class CJasonBullet : public CBullet
{
private:
	const DWORD aliveDuration = 4000;

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

