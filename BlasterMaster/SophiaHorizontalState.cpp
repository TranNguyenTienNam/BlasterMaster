#include "SophiaHorizontalState.h"

void CSophiaHorizontalState::Shooting()
{
}

void CSophiaHorizontalState::UpdateColliders()
{
	int nx = owner->GetDirection();
	auto colliders = owner->GetColliders();

	colliders.clear();

	auto collider = new CCollider2D;
	collider->SetGameObject(owner);
	collider->SetOffset(OFFSET_SOPHIA_IDLE);
	collider->SetBoxSize(BOX_SOPHIA_IDLE);
	collider->SetDynamic(true);
	colliders.push_back(collider);

	owner->SetColliders(colliders);
}

void CSophiaHorizontalState::Update(DWORD dt)
{
	int nx = owner->GetDirection();
	owner->leftWheel->SetPosition(Vector2(-8.0f, 0.0f));
	owner->rightWheel->SetPosition(Vector2(8.0f, 0.0f));
	owner->middle->SetPosition(Vector2(0.0f, 1.0f));
	owner->cabin->SetPosition(Vector2(-3.5f * nx, 9.0f));
	owner->gun->SetPosition(Vector2(8.0f * nx, 9.0f));
}

void CSophiaHorizontalState::Render()
{
}