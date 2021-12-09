#include "Enemy.h"
#include "PowerItem.h"

void CEnemy::DropItem()
{
}

void CEnemy::OnDestroy()
{
	auto item = Instantiate<CPowerItem>(transform.position);
}
