#pragma once
#include "GameObject.h"
#include "Playable.h"

class CEnemy : public CGameObject
{
protected:
	static CPlayable* target;

	int hp;
	int damageOnCollision;
public:
	CEnemy();
	void DropItem();
	void OnDestroy();
};

