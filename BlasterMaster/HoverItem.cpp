#include "HoverItem.h"

void CHoverItem::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("hover-item", sprites->Get("spr-hover-item"));
}

CHoverItem::CHoverItem()
{
	InitSprites();
}

void CHoverItem::GetEffect()
{
	DebugOut(L"Player picked a hover item up!!!\n");
}

void CHoverItem::Render()
{
	sprites.at("hover-item")->Draw(transform.position, 1, layer_index);
}
