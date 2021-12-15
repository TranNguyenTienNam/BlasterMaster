#include "NeowormLarva.h"
#include "Brick.h"
#include "Playable.h"
#include "Neoworm.h"
#include "BigExplosion.h"

void CNeowormLarva::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("Larva", sprites->Get("spr-neoworm-larva"));
}

void CNeowormLarva::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

void CNeowormLarva::Explosion()
{
}

CNeowormLarva::CNeowormLarva()
{
	InitSprites();
	InitColliders();

	tag = ObjectTag::EnemyBullet;
}

CNeowormLarva::~CNeowormLarva()
{
}

void CNeowormLarva::Update(DWORD dt)
{
	velocity.y += -0.00026f * dt;
}

void CNeowormLarva::Render()
{
	sprites.at("Larva")->Draw(transform.position, nx, layer_index);
}

void CNeowormLarva::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	if (hasHatched == true) return;

	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other) || dynamic_cast<CPlayable*>(other))
	{
		auto neoworm = Instantiate<CNeoworm>(transform.position);
		neoworm->SetVelocity(Vector2(nx * 0.05f, 0.08f));

		Instantiate<CBigExplosion>(transform.position);
		hasHatched = true;

		isDestroyed = true;
		isEnabled = false;
	}
}
