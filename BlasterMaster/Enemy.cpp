#include "Enemy.h"
#include "PowerItem.h"
#include "HoverItem.h"
#include "Math.h"

CEnemy::CEnemy()
{
	tag = ObjectTag::Enemy;
}

void CEnemy::DropItem()
{
	// TODO: Can only drop a single item, so convert each item's own rate to its rate in all items
	if (CMath::Random() <= CAbstractItem::GetDropRate())
	{
		auto item = Instantiate<CHoverItem>(transform.position);
	}
}

void CEnemy::OnDestroy()
{
	DropItem();
	DebugOut(L"drop item\n");
}
