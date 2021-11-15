#include "SophiaIdleState.h"

void CSophiaIdleState::Shooting()
{
}

void CSophiaIdleState::Update(DWORD dt, CSophia& sophia, int nx)
{
	sophia.posLeftWheel = Vector2(-8.0f, 0.0f);
	sophia.posRightWheel = Vector2(8.0f, 0.0f);
	sophia.posMiddle = Vector2(0.0f, 1.0f);
	sophia.posCabin = Vector2(-3.5f * nx, 9.0f);
	sophia.posGun = Vector2(8.0f * nx, 9.0f);

	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	sophia.sprCabin = sprites->Get("spr-sophia-cabin");
	sophia.sprGun = sprites->Get("spr-sophia-gun-00");
}

void CSophiaIdleState::Render()
{
}