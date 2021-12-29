#include "SophiaGun.h"
#include "SophiaHorizontalState.h"
#include "SophiaUpward45State.h"
#include "SophiaUpwardState.h"

void CSophiaGun::InitSprites()
{
	auto sprite_manager = CGame::GetInstance()->GetService<CSprites>();
	AddSprite(SPR_GUN_00, sprite_manager->Get("spr-sophia-gun-00"));
	AddSprite(SPR_GUN_45, sprite_manager->Get("spr-sophia-gun-45"));
	AddSprite(SPR_GUN_90, sprite_manager->Get("spr-sophia-gun-90"));
}

CSophiaGun::CSophiaGun(CSophia* sophia)
{
	parent = sophia;
	InitSprites();
}

CSophiaGun::~CSophiaGun()
{
}

void CSophiaGun::Update(DWORD dt)
{
	nx = parent->GetDirection();
}

void CSophiaGun::Render()
{
	auto directionState = parent->GetDirectionState();
	if (dynamic_cast<CSophiaHorizontalState*>(directionState)->IsTurning() == true) return;

	if (dynamic_cast<CSophiaHorizontalState*>(directionState))
		sprites.at(SPR_GUN_00)->Draw(transform.position + parent->GetPosition(), -nx, layer_index);
	else if (dynamic_cast<CSophiaUpward45State*>(directionState))
		sprites.at(SPR_GUN_45)->Draw(transform.position + parent->GetPosition(), -nx, layer_index);
	else if (dynamic_cast<CSophiaUpwardState*>(directionState))
		sprites.at(SPR_GUN_90)->Draw(transform.position + parent->GetPosition(), -nx, layer_index);
}