#include "BallCarry.h"
#include "Animations.h"

void CBallCarry::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("BallCarry", animations->Get("ani-ballcarry"));
}

void CBallCarry::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CBallCarry::CBallCarry()
{
	InitAnimations();
	InitColliders();
}

CBallCarry::~CBallCarry()
{
}

void CBallCarry::Update(DWORD dt)
{
}

void CBallCarry::Render()
{
	animations.at("BallCarry")->Render(transform.position, nx);
}
