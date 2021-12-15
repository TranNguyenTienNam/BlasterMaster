#pragma once
#include "Bullet.h"
class CBigJasonBullet : public CBullet
{
private:
	const std::string BULLET_X = "BigJason-XBullet";
	const std::string BULLET_Y = "BigJason-YBullet";
	const Vector2 BOX_X = Vector2(8.0f, 6.0f);
	const Vector2 BOX_Y = Vector2(6.0f, 8.0f);
protected:
	void InitSprites();
public:
	CBigJasonBullet();
	~CBigJasonBullet();

	virtual void Update(DWORD dt);
	virtual void Render();

	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

