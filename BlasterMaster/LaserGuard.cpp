#include "LaserGuard.h"
#include "Animations.h"
#include "Brick.h"
#include "LaserBullet.h"

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
	lastTimeShooting = GetTickCount();

	leftEdge = 9999999;
	rightEdge = -9999999;
}

CLaserGuard::~CLaserGuard()
{
}

void CLaserGuard::Update(DWORD dt)
{
	if (target == nullptr) return;

	if (transform.position.x < leftEdge) leftEdge = transform.position.x;
	if (transform.position.x > rightEdge) rightEdge = transform.position.x;

	auto targetPos = target->GetPosition();
	if (targetPos.y < transform.position.y && 
		targetPos.x < rightEdge &&
		targetPos.x > leftEdge)
	{
		DWORD now = GetTickCount();
		if (now - lastTimeShooting > shootingDelay)
		{
			lastTimeShooting = now;

			auto direction = CMath::Normalize(targetPos - transform.position);

			auto bullet = Instantiate<CLaserBullet>(transform.position);
			bullet->SetVelocity(direction * bullet->GetSpeed());
		}
	}	

	OnDamagedUpdate();
}

void CLaserGuard::Render()
{
	animations.at("LaserGuard")->Render(transform.position, 1, layer_index, 0, damagedColor[colorIndex]);
}

void CLaserGuard::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other))
	{
		velocity.x = collision->nx * MOVE_SPEED;
		if (collision->nx == 1)
		{
			leftEdge = transform.position.x;
		}

		if (collision->nx == -1)
		{
			rightEdge = transform.position.x;
		}
	}
}
