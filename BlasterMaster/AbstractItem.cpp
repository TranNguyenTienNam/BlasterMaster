#include "AbstractItem.h"

int CAbstractItem::dropRate = 100;

CAbstractItem::CAbstractItem()
{
	// Init colliders
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetTrigger(true);
	collider->SetDynamic(true);
	colliders.push_back(collider);

	tag = ObjectTag::Item;

	timeSpawn = GetTickCount();
	flickeringDuration = (aliveDuration - preWarningDuration) / flickeringTimes;
}

void CAbstractItem::GetEffect(CPlayable* picker)
{
}

void CAbstractItem::Update(DWORD dt)
{
	DWORD now = GetTickCount();

	if (now - timeSpawn > preWarningDuration)
	{
		int index = (now - timeSpawn - preWarningDuration) / flickeringDuration % alphaArray.size();
		alpha = alphaArray[index];
	}

	if (now - timeSpawn > aliveDuration)
	{
		SetDestroyed();
		SetEnable(false);
	}
}

void CAbstractItem::Render()
{
}

void CAbstractItem::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CPlayable*>(other))
	{
		GetEffect((CPlayable*)other);
		SetDestroyed();
		SetEnable(false);
	}
}
