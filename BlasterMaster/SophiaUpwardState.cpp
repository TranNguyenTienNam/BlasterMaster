#include "SophiaUpwardState.h"

void CSophiaUpwardState::Shooting()
{
}

void CSophiaUpwardState::UpdateColliders()
{
	int nx = owner->GetDirection();
	auto colliders = owner->GetColliders();

	colliders.clear();

	auto collider = new CCollider2D;
	collider->SetGameObject(owner);
	collider->SetOffset(Vector2(-3.0f * nx, 12.0f));
	collider->SetBoxSize(BOX_SOPHIA_UPWARD);
	collider->SetDynamic(true);
	colliders.push_back(collider);

	owner->SetColliders(colliders);
}

void CSophiaUpwardState::Update(DWORD dt)
{
	int nx = owner->GetDirection();
	owner->leftWheel->SetPosition(Vector2(-5.0f, 0.0f));
	owner->rightWheel->SetPosition(Vector2(5.0f, 0.0f));
	owner->middle->SetPosition(Vector2(0.0f, 7.0f));
	owner->cabin->SetPosition(Vector2(-7.0f * nx, 13.0f));
	owner->gun->SetPosition(Vector2(-3.0f * nx, 24.0f));
}

void CSophiaUpwardState::Render()
{
	
}
