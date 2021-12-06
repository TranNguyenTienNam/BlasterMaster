#include "GX680.h"
#include "Animations.h"

void CGX680::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("GX680", animations->Get("ani-gx680"));
}

void CGX680::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CGX680::CGX680()
{
	InitAnimations();
	InitColliders();
}

CGX680::~CGX680()
{
}

void CGX680::Update(DWORD dt)
{
}

void CGX680::Render()
{
	animations.at("GX680")->Render(transform.position, 1);
}