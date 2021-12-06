#include "Eyelet.h"
#include "Animations.h"

void CEyelet::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Eyelet", animations->Get("ani-eyelet"));
}

void CEyelet::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CEyelet::CEyelet()
{
	InitAnimations();
	InitColliders();
}

CEyelet::~CEyelet()
{
}

void CEyelet::Update(DWORD dt)
{
}

void CEyelet::Render()
{
	animations.at("Eyelet")->Render(transform.position, nx);
}