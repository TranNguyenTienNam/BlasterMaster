#include "BallBomb.h"
#include "Brick.h"
#include "Playable.h"

void CBallBomb::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("BallBomb", sprites->Get("spr-ballcarry-bomb"));
}

void CBallBomb::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

void CBallBomb::Explosion()
{
}

CBallBomb::CBallBomb()
{
	InitSprites();
	InitColliders();

	tag = ObjectTag::EnemyBullet;

	bouncingCount = 0;
	bouncingPush = 0.08f;

	velocity.y = 0.12f;
}

CBallBomb::~CBallBomb()
{
}

void CBallBomb::Update(DWORD dt)
{
	velocity.y += -0.00026f * dt;
}

void CBallBomb::Render()
{
	sprites.at("BallBomb")->Draw(transform.position, nx, layer_index);
}

void CBallBomb::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other))
	{
		if (bouncingCount > maxBouncingCount)
		{
			Explosion();
		}
		else
		{
			velocity.y = bouncingPush;

			bouncingPush -= 0.016f;
			if (bouncingPush < 0) bouncingPush = 0;

			bouncingCount++;
		}
	}
	else if (dynamic_cast<CPlayable*>(other))
	{
		Explosion();
	}
}
