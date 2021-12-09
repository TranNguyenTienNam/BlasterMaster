#include "PowerItem.h"

void CPowerItem::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("power-item", sprites->Get("spr-power-item"));
}

CPowerItem::CPowerItem()
{
	/*auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetTrigger(true);
	colliders.push_back(collider);

	timeSpawn = GetTickCount();*/

	InitSprites();
}

void CPowerItem::GetEffect()
{
	DebugOut(L"Player picked a power item up!!!\n");
}

void CPowerItem::Render()
{
	sprites.at("power-item")->Draw(transform.position, 1);
}
