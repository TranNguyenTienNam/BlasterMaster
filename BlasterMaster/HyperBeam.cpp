#include "HyperBeam.h"
#include "Enemy.h"
#include "Brick.h"
#include "SmallExplosion.h"
#include "Sound.h"

void CHyperBeam::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite(HYPERBEAM_X, sprites->Get("spr-hyperbeam-axisx"));
	AddSprite(HYPERBEAM_Y, sprites->Get("spr-hyperbeam-axisy"));
}

CHyperBeam::CHyperBeam()
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

CHyperBeam::~CHyperBeam()
{
}

void CHyperBeam::Update(DWORD dt)
{
	if (velocity.x != 0) colliders.at(0)->SetBoxSize(BOX_X);
	else if (velocity.y != 0) colliders.at(0)->SetBoxSize(BOX_Y);

	DWORD now = GetTickCount();
	if (now - initTime > aliveDuration)
	{
		isEnabled = false;
		isDestroyed = true;

		CGame::GetInstance()->GetService<CSound>()->PlayWaveFile("BulletExplosion");
	}
}

void CHyperBeam::Render()
{
	if (velocity.x != 0) sprites.at(HYPERBEAM_X)->Draw(transform.position, -nx, layer_index);
	else if (velocity.y != 0) sprites.at(HYPERBEAM_Y)->Draw(transform.position, 1, layer_index);
}

void CHyperBeam::OnOverlapped(CCollider2D* selfCollider, CGameObject* object)
{
	if (dynamic_cast<CEnemy*>(object))
	{
		((CEnemy*)object)->TakeDamage(damage);

		isEnabled = false;
		isDestroyed = true;
	}
	else if (dynamic_cast<CBrick*>(object))
	{
		isEnabled = false;
		isDestroyed = true;

		Instantiate<CSmallExplosion>(transform.position);
	}
}

void CHyperBeam::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
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

		Instantiate<CSmallExplosion>(transform.position);
	}
}
