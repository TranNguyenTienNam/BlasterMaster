#include "HyperBeam.h"
#include "Enemy.h"

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
}

CHyperBeam::~CHyperBeam()
{
}

void CHyperBeam::Update(DWORD dt)
{
	if (velocity.x != 0) colliders.at(0)->SetBoxSize(BOX_X);
	else if (velocity.y != 0) colliders.at(0)->SetBoxSize(BOX_Y);
}

void CHyperBeam::Render()
{
	if (velocity.x != 0) sprites.at(HYPERBEAM_X)->Draw(transform.position, -nx);
	else if (velocity.y != 0) sprites.at(HYPERBEAM_Y)->Draw(transform.position, 1);
}

void CHyperBeam::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
}

void CHyperBeam::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CEnemy*>(other))
	{
		((CEnemy*)other)->OnDestroy();
		other->SetDestroyed();
		other->SetEnable(false);
	}

	SetDestroyed();
	SetEnable(false);

	// TODO: Instantiate VFX explosion
}
