#include "Stuka.h"
#include "Animations.h"

void CStuka::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Stuka", animations->Get("ani-stuka"));
}

void CStuka::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CStuka::CStuka()
{
	InitAnimations();
	InitColliders();
}

CStuka::~CStuka()
{
}

void CStuka::Update(DWORD dt)
{
}

void CStuka::Render()
{
	animations.at("Stuka")->Render(transform.position, nx);
}