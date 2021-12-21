#include "LaserGuard.h"
#include "Animations.h"
#include "Brick.h"

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

	velocity.x = MOVE_SPEED;
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

void CLaserGuard::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other))
	{
		velocity.x = collision->nx * MOVE_SPEED;
	}
}
