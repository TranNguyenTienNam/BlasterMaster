#include "JasonBullet.h"
#include "Enemy.h"
#include "Brick.h"

void CJasonBullet::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("Jason-Bullet", sprites->Get("spr-jason-bullet"));
}

CJasonBullet::CJasonBullet()
{
	InitSprites();

	// Init collider
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetDynamic(true);
	collider->SetTrigger(true);
	colliders.push_back(collider);

	tag = ObjectTag::PlayerBullet;

	initTime = GetTickCount();
}

CJasonBullet::~CJasonBullet()
{
}

void CJasonBullet::Update(DWORD dt)
{
	DWORD now = GetTickCount();
	if (now - initTime > aliveDuration)
	{
		isEnabled = false;
		isDestroyed = true;
	}
}

void CJasonBullet::Render()
{
	sprites.at("Jason-Bullet")->Draw(transform.position, 1, layer_index);
}

void CJasonBullet::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CEnemy*>(other))
	{
		((CEnemy*)other)->TakeDamage(damage);

		isEnabled = false;
		isDestroyed = true;
	}
	else if (dynamic_cast<CBrick*>(other))
	{
		isEnabled = false;
		isDestroyed = true;
	}
}
