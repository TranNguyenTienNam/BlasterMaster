#pragma once
#include "GameObject.h"
#include "Playable.h"

class CEnemy : public CGameObject
{
protected:
	static CPlayable* target;

	const int maxHP = 2;

	bool scrollingMap = true;
	int hp;
	int damageOnCollision = 1;
public:
	CEnemy();
	static CPlayable* GetTarget() { return target; }
	static void SetTarget(CPlayable* object) { target = object; }
	void DropItem();
	void OnDestroy();

	int GetDamageOnCollision() { return this->damageOnCollision * -1; }
	virtual void TakeDamage(int damage);
};

