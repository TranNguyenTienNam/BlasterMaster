#include "AbstractItem.h"
#include "Playable.h"

CAbstractItem::CAbstractItem()
{
	// Init colliders
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetTrigger(true);
	collider->SetDynamic(true);
	colliders.push_back(collider);

	timeSpawn = GetTickCount();
}

void CAbstractItem::GetEffect()
{
}

void CAbstractItem::Update(DWORD dt)
{
	DWORD now = GetTickCount();
	if (now - timeSpawn > aliveDuration)
	{
		// TODO: Flickering effect before despawning
		SetDestroyed();
		SetEnable(false);
	}
}

void CAbstractItem::Render()
{
}

void CAbstractItem::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	if (dynamic_cast<CPlayable*>(collision->obj))
	{
		// TODO: Set collider of all items is static, then move SetDestroyed and SetEnable function into GetEffect
		GetEffect();
		SetDestroyed();
		SetEnable(false);
	}
}
