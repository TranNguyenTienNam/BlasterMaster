#pragma once
#include "Bullet.h"

#define HYPERBEAM_X "HyperBeam-X"
#define HYPERBEAM_Y "HyperBeam-Y"

#define BOX_X Vector2(24.0f, 6.0f)
#define BOX_Y Vector2(6.0f, 24.0f)

class CHyperBeam : public CBullet
{
protected:
	void InitSprites();
public:
	CHyperBeam();
	~CHyperBeam();

	virtual void Update(DWORD dt);
	virtual void Render();

	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

