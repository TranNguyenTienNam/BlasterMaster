#include "SophiaUpward45State.h"

void CSophiaUpward45State::Shooting()
{
}

void CSophiaUpward45State::UpdateColliders()
{
	int nx = owner->GetDirection();
	auto colliders = owner->GetColliders();

	colliders.clear();

	auto collider = new CCollider2D;
	collider->SetGameObject(owner);
	collider->SetOffset(Vector2(-0.5f * nx, 10.5f));
	collider->SetBoxSize(BOX_SOPHIA_UPWARD_45);
	collider->SetDynamic(true);
	colliders.push_back(collider);

	owner->SetColliders(colliders);
}

void CSophiaUpward45State::Update(DWORD dt)
{
	int nx = owner->GetDirection();
	owner->leftWheel->SetPosition(Vector2(-6.5f, 0.0f));
	owner->rightWheel->SetPosition(Vector2(6.5f, 0.0f));
	owner->middle->SetPosition(Vector2(0.0f, 6.0f));
	owner->cabin->SetPosition(Vector2(-3.5f * nx, 14.0f));
	owner->gun->SetPosition(Vector2(8.0f * nx, 21.0f));
}

void CSophiaUpward45State::Render()
{
}