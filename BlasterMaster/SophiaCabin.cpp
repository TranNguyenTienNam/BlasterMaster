#include "SophiaCabin.h"
#include "SophiaHorizontalState.h"
#include "SophiaUpward45State.h"
#include "SophiaUpwardState.h"

void CSophiaCabin::InitSprites()
{
	auto sprite_manager = CGame::GetInstance()->GetService<CSprites>();
	AddSprite(SPR_CABIN_00, sprite_manager->Get("spr-sophia-cabin"));
	AddSprite(SPR_CABIN_UPWARD, sprite_manager->Get("spr-sophia-cabin-45"));
	AddSprite(SPR_CABIN_TURN, sprite_manager->Get("spr-sophia-cabin-turn"));
}

CSophiaCabin::CSophiaCabin(CSophia* sophia)
{
	parent = sophia;
	InitSprites();
	nx = parent->GetDirection();
}

CSophiaCabin::~CSophiaCabin()
{
}

void CSophiaCabin::Update(DWORD dt)
{
}

void CSophiaCabin::Render()
{
	auto directionState = parent->GetDirectionState();
	if (dynamic_cast<CSophiaHorizontalState*>(directionState))
	{
		if (dynamic_cast<CSophiaHorizontalState*>(directionState)->IsTurning() == false)
		{
			sprites.at(SPR_CABIN_00)->Draw(transform.position + parent->GetPosition(), -nx, layer_index);
		}
		else
		{
			sprites.at(SPR_CABIN_TURN)->Draw(transform.position + parent->GetPosition(), -nx, layer_index);
		}
	}
	else
	{
		sprites.at(SPR_CABIN_UPWARD)->Draw(transform.position + parent->GetPosition(), -nx, layer_index);
	}
}