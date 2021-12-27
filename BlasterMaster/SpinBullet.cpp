#include "SpinBullet.h"
#include "Playable.h"

void CSpinBullet::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("Laser", sprites->Get("spr-z88-bullet"));
}

void CSpinBullet::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	collider->SetTrigger(true);
	colliders.push_back(collider);
}

CSpinBullet::CSpinBullet()
{
	InitSprites();
	InitColliders();

	tag = ObjectTag::EnemyBullet;
	lastTimeSpin = GetTickCount();
}

CSpinBullet::~CSpinBullet()
{
}

void CSpinBullet::Update(DWORD dt)
{
	DWORD now = GetTickCount();
	if (now - lastTimeSpin > spinSpeed)
	{
		if (velocity.x < 0) angleZ -= 45;
		else angleZ += 45;

		if (angleZ > 360) angleZ = 0;

		lastTimeSpin = now;
	}
}

void CSpinBullet::Render()
{
	sprites.at("Laser")->Draw(transform.position, nx, layer_index, angleZ);
}

void CSpinBullet::OnOverlapped(CCollider2D* selfCollider, CGameObject* object)
{
	if (dynamic_cast<CPlayable*>(object))
	{
		isDestroyed = true;
		isEnabled = false;

		((CPlayable*)object)->AffectPowerAttribute(-damage);
	}
}

void CSpinBullet::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CPlayable*>(other))
	{
		isDestroyed = true;
		isEnabled = false;

		((CPlayable*)other)->AffectPowerAttribute(-damage);
	}
}
