#pragma once
#include "GameObject.h"
#include "Playable.h"

class CEnemy : public CGameObject
{
protected:
	static CPlayable* target;

	const int maxHP = 2;

	int hp;
	int damageOnCollision = 1;
public:
	CEnemy();
	static void SetTarget(CPlayable* object) { target = object; }
	void DropItem();
	void OnDestroy();
	void TakeDamage(int damage);
};

