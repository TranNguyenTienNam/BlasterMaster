#include "GX680Bullet.h"
#include "Brick.h"
#include "Playable.h"

void CGX680Bullet::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("Bullet", sprites->Get("spr-gx680-bullet"));
}

void CGX680Bullet::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CGX680Bullet::CGX680Bullet()
{
	InitSprites();
	InitColliders();

	tag = ObjectTag::EnemyBullet;
}

CGX680Bullet::~CGX680Bullet()
{
}

void CGX680Bullet::Update(DWORD dt)
{
}

void CGX680Bullet::Render()
{
	sprites.at("Bullet")->Draw(transform.position, nx, layer_index);
}

void CGX680Bullet::OnOverlapped(CCollider2D* selfCollider, CGameObject* object)
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

void CGX680Bullet::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
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
