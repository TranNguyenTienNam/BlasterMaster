#include "Drap.h"
#include "Animations.h"
#include "Utils.h"
#include "Transform.h"

void CDrap::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Clockwise", animations->Get("ani-drap"));
}

void CDrap::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	colliders.push_back(collider);
}

CDrap::CDrap()
{
	InitAnimations();
	InitColliders();
}

CDrap::~CDrap()
{
}

void CDrap::Update(DWORD dt)
{
}

void CDrap::Render()
{
	Vector2 pos = transform.position;
	animations.at("Clockwise")->Render(pos, nx, layer_index);
}