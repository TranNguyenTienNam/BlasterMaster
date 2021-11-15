#include "SophiaUpward45State.h"

void CSophiaUpward45State::Shooting()
{
}

void CSophiaUpward45State::Update(DWORD dt, CSophia& sophia, int nx)
{
	sophia.posLeftWheel = Vector2(-6.5f, 0.0f);
	sophia.posRightWheel = Vector2(6.5f, 0.0f);
	sophia.posMiddle = Vector2(0.0f, 6.0f);
	sophia.posCabin = Vector2(-3.5f * nx, 14.0f);
	sophia.posGun = Vector2(8.0f * nx, 21.0f);

	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	sophia.sprCabin = sprites->Get("spr-sophia-cabin");
	sophia.sprGun = sprites->Get("spr-sophia-gun-45");
}

void CSophiaUpward45State::Render()
{
}