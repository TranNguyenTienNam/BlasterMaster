#include "SophiaMiddle.h"

void CSophiaMiddle::InitSprites()
{
	auto sprite_manager = CGame::GetInstance()->GetService<CSprites>();
	AddSprite(SPR_MIDDLE, sprite_manager->Get("spr-sophia-middle"));
}

CSophiaMiddle::CSophiaMiddle(CSophia* sophia)
{
	parent = sophia;
	InitSprites();
}

CSophiaMiddle::~CSophiaMiddle()
{
}

void CSophiaMiddle::Update(DWORD dt)
{
	nx = parent->GetDirection();
}

void CSophiaMiddle::Render()
{
	sprites.at(SPR_MIDDLE)->Draw(transform.position + parent->GetPosition(), -nx, layer_index);
}
