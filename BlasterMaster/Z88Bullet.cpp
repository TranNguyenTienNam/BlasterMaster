#include "Z88Bullet.h"
#include "Playable.h"

void CZ88Bullet::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("Laser", sprites->Get("spr-z88-bullet"));
}

void CZ88Bullet::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetDynamic(true);
	collider->SetTrigger(true);
	colliders.push_back(collider);
}

CZ88Bullet::CZ88Bullet()
{
	InitSprites();
	InitColliders();

	tag = ObjectTag::EnemyBullet;
}

CZ88Bullet::~CZ88Bullet()
{
}

void CZ88Bullet::Update(DWORD dt)
{
}

void CZ88Bullet::Render()
{
	sprites.at("Laser")->Draw(transform.position, nx, layer_index, angleZ);
}

void CZ88Bullet::OnOverlapped(CCollider2D* selfCollider, CGameObject* object)
{
	if (dynamic_cast<CPlayable*>(object))
	{
		isDestroyed = true;
		isEnabled = false;

		((CPlayable*)object)->AffectPowerAttribute(-damage);
	}
}

void CZ88Bullet::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CPlayable*>(other))
	{
		isDestroyed = true;
		isEnabled = false;

		((CPlayable*)other)->AffectPowerAttribute(-damage);
	}
}
