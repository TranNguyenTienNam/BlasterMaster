#include "LaserBullet.h"
#include "Brick.h"
#include "Playable.h"

void CLaserBullet::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("Laser", sprites->Get("spr-laser-bullet"));
}

void CLaserBullet::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CLaserBullet::CLaserBullet()
{
	InitSprites();
	InitColliders();

	tag = ObjectTag::EnemyBullet;
}

CLaserBullet::~CLaserBullet()
{
}

void CLaserBullet::Update(DWORD dt)
{
}

void CLaserBullet::Render()
{
	sprites.at("Laser")->Draw(transform.position, nx, layer_index);
}

void CLaserBullet::OnOverlapped(CCollider2D* selfCollider, CGameObject* object)
{
	if (dynamic_cast<CBrick*>(object) || dynamic_cast<CPlayable*>(object))
	{
		isDestroyed = true;
		isEnabled = false;

		if (dynamic_cast<CPlayable*>(object))
		{
			((CPlayable*)object)->AffectPowerAttribute(-damage);
		}
	}
}

void CLaserBullet::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other) || dynamic_cast<CPlayable*>(other))
	{
		isDestroyed = true;
		isEnabled = false;

		if (dynamic_cast<CPlayable*>(other))
		{
			((CPlayable*)other)->AffectPowerAttribute(-damage);
		}
	}
}
