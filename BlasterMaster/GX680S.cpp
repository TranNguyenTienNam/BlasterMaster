#include "GX680S.h"
#include "Animations.h"

void CGX680S::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("GX680S", animations->Get("ani-gx680s"));
}

void CGX680S::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CGX680S::CGX680S()
{
	InitAnimations();
	InitColliders();
}

CGX680S::~CGX680S()
{
}

void CGX680S::Update(DWORD dt)
{
}

void CGX680S::Render()
{
	animations.at("GX680S")->Render(transform.position, 1);
}
