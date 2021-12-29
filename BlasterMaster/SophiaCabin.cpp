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
	AddSprite(SPR_CABIN_OPEN, sprite_manager->Get("spr-sophia-cabin-open"));
}

CSophiaCabin::CSophiaCabin(CSophia* sophia)
{
	parent = sophia;
	InitSprites();
	nx = parent->GetDirection();
	isSwitching = false;
}

CSophiaCabin::~CSophiaCabin()
{
}

void CSophiaCabin::Switching()
{
	isSwitching = true;
	lastTimeSwitching = GetTickCount();
}

void CSophiaCabin::Update(DWORD dt)
{
	if (isSwitching == true)
	{
		DWORD now = GetTickCount();
		if (now - lastTimeSwitching > switchingDuration)
		{
			lastTimeSwitching = now;
			isSwitching = false;
		}
	}
}

void CSophiaCabin::Render()
{
	int colorIndex = parent->GetColorIndex();
	auto directionState = parent->GetDirectionState();
	if (dynamic_cast<CSophiaHorizontalState*>(directionState))
	{
		if (isSwitching == true)
		{
			sprites.at(SPR_CABIN_OPEN)->Draw(transform.position + parent->GetPosition() + Vector2(0.0f, 4.0f), -nx, layer_index, 0, damagedColor[colorIndex]);
		}
		else
		{
			if (dynamic_cast<CSophiaHorizontalState*>(directionState)->IsTurning() == false)
			{
				sprites.at(SPR_CABIN_00)->Draw(transform.position + parent->GetPosition(), -nx, layer_index, 0, damagedColor[colorIndex]);
			}
			else
			{
				sprites.at(SPR_CABIN_TURN)->Draw(transform.position + parent->GetPosition(), -nx, layer_index, 0, damagedColor[colorIndex]);
			}
		}
	}
	else
	{
		nx = parent->GetDirection();
		sprites.at(SPR_CABIN_UPWARD)->Draw(transform.position + parent->GetPosition(), -nx, layer_index, 0, damagedColor[colorIndex]);
	}
}