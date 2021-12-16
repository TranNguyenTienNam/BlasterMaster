#include "LaserGuard.h"
#include "Animations.h"

void CLaserGuard::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("LaserGuard", animations->Get("ani-laserguard"));
}

void CLaserGuard::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CLaserGuard::CLaserGuard()
{
	InitAnimations();
	InitColliders();

	scrollingMap = false;
}

CLaserGuard::~CLaserGuard()
{
}

void CLaserGuard::Update(DWORD dt)
{
}

void CLaserGuard::Render()
{
	animations.at("LaserGuard")->Render(transform.position, 1, layer_index);
}
