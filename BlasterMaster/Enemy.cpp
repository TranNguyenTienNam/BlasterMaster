#include "Enemy.h"
#include "PowerItem.h"
#include "HoverItem.h"
#include "Math.h"
#include "BigExplosion.h"

CPlayable* CEnemy::target = nullptr;

CEnemy::CEnemy()
{
	tag = ObjectTag::Enemy;
	hp = maxHP;
}

void CEnemy::DropItem()
{
	// TODO: Can only drop a single item, so convert each item's own rate to its rate in all items
	if (CMath::Random(1, 100) <= CAbstractItem::GetDropRate())
	{
		auto item = Instantiate<CPowerItem>(transform.position);
	}
}

void CEnemy::OnDestroy()
{
	DropItem();

	isEnabled = false;
	isDestroyed = true;

	Instantiate<CBigExplosion>(transform.position);
}

void CEnemy::TakeDamage(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		hp = 0;
		OnDestroy();
	}
}
