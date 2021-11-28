#include "Drap.h"
#include "Animations.h"
#include "Utils.h"
#include "Transform.h"

void CDrap::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Clockwise", animations->Get("ani-drap"));
}

CDrap::CDrap() :CGameObject()
{
	InitAnimations();

	// Init collider
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DRAP_SIZE);
	colliders.push_back(collider);
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
	animations.at("Clockwise")->Render(pos, nx);
}