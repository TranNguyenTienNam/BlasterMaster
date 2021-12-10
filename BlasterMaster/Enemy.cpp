#include "Enemy.h"
#include "PowerItem.h"
#include "HoverItem.h"

CEnemy::CEnemy()
{
	tag = ObjectTag::Enemy;
}

void CEnemy::DropItem()
{
}

void CEnemy::OnDestroy()
{
	auto item = Instantiate<CHoverItem>(transform.position);
}
