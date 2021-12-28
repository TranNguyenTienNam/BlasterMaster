#include "SophiaHorizontalState.h"
#include "HyperBeam.h"
#include "Sound.h"

void CSophiaHorizontalState::Shooting()
{
	auto newBullet = Instantiate<CHyperBeam>(owner->GetPosition() + owner->gun->GetPosition());
	int nx = owner->GetDirection();
	newBullet->SetDirection(nx);
	newBullet->SetVelocity(Vector2(nx * newBullet->GetSpeed(), 0.0f));

	CGame::GetInstance()->GetService<CSound>()->PlayWaveFile("SophiaBullet");
}

void CSophiaHorizontalState::UpdateColliders()
{
	int nx = owner->GetDirection();
	auto colliders = owner->GetColliders();

	colliders.at(0)->SetOffset(OFFSET_SOPHIA_IDLE);
	colliders.at(0)->SetBoxSize(BOX_SOPHIA_IDLE);
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