#include "Enemy.h"
#include "PowerItem.h"
#include "HoverItem.h"
#include "Math.h"
#include "BigExplosion.h"
#include "Firework.h"
#include "Sound.h"

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

	if (scrollingMap == true) Instantiate<CBigExplosion>(transform.position);
	else Instantiate<CFirework>(transform.position);
}

void CEnemy::TakeDamage(int damage)
{
	CGame::GetInstance()->GetService<CSound>()->PlayWaveFile("EnemyOnDamaged");

	hp -= damage;
	if (hp <= 0)
	{
		hp = 0;
		OnDestroy();

		CGame::GetInstance()->GetService<CSound>()->PlayWaveFile("EnemyDie");
	}
}
