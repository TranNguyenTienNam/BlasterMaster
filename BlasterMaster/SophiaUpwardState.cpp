#include "SophiaUpwardState.h"
#include "HyperBeam.h"

void CSophiaUpwardState::Shooting()
{
	auto newBullet = Instantiate<CHyperBeam>(owner->GetPosition() + owner->gun->GetPosition());
	newBullet->SetVelocity(Vector2(0.0f, newBullet->GetSpeed()));
}

void CSophiaUpwardState::UpdateColliders()
{
	int nx = owner->GetDirection();
	auto colliders = owner->GetColliders();

	colliders.at(0)->SetOffset(Vector2(-3.0f * nx, 12.0f));
	colliders.at(0)->SetBoxSize(BOX_SOPHIA_UPWARD);

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
