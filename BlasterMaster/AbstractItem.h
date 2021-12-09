#pragma once
#include "GameObject.h"

class CAbstractItem : public CGameObject
{
protected:
	const DWORD aliveDuration = 5000;
	DWORD timeSpawn;
public:
	CAbstractItem();
	virtual void GetEffect();
	virtual void Update(DWORD dt);
	virtual void Render();
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

