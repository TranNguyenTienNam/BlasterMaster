#include "Enemy.h"
#include "PowerItem.h"
#include "HoverItem.h"

void CEnemy::DropItem()
{
}

void CEnemy::OnDestroy()
{
	auto item = Instantiate<CHoverItem>(transform.position);
}
