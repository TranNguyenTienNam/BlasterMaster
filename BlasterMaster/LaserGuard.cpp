#include "LaserGuard.h"
#include "Animations.h"

void CLaserGuard::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("LaserGuard", animations->Get("ani-laserguard"));
}

CLaserGuard::CLaserGuard()
{
	InitAnimations();
}

CLaserGuard::~CLaserGuard()
{
}

void CLaserGuard::Update(DWORD dt)
{
}

void CLaserGuard::Render()
{
	animations.at("LaserGuard")->Render(transform.position, 1);
}
