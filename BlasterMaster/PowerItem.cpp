#include "PowerItem.h"

void CPowerItem::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("power-item", sprites->Get("spr-power-item"));
}

CPowerItem::CPowerItem()
{
	InitSprites();
}

void CPowerItem::GetEffect(CPlayable* picker)
{
	DebugOut(L"Player picked a power item up!!!\n");
	picker->AffectPowerAttribute(1);
}

void CPowerItem::Render()
{
	sprites.at("power-item")->Draw(transform.position, 1, layer_index, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}
