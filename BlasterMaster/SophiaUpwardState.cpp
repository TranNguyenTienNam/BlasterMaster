#include "SophiaUpwardState.h"

void CSophiaUpwardState::Shooting()
{
}

void CSophiaUpwardState::Update(DWORD dt, CSophia& sophia, int nx)
{
	sophia.posLeftWheel = Vector2(-5.0f, 0.0f);
	sophia.posRightWheel = Vector2(5.0f, 0.0f);
	sophia.posMiddle = Vector2(0.0f, 7.0f);
	sophia.posCabin = Vector2(-7.0f * nx, 13.0f);
	sophia.posGun = Vector2(-3.0f * nx, 24.0f);

	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	sophia.sprCabin = sprites->Get("spr-sophia-cabin-45");
	sophia.sprGun = sprites->Get("spr-sophia-gun-90");
}

void CSophiaUpwardState::Render()
{
	
}
