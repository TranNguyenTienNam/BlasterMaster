#include "Stuka.h"
#include "Animations.h"

void CStuka::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Stuka", animations->Clone("ani-stuka"));
}

void CStuka::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	collider->SetTrigger(true);
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

void CStuka::SetDirection(int nx)
{
	velocity.x = nx * MOVE_SPEED;
	if (nx < 0)
	{
		animations.at("Stuka")->SetIsReversed(true);
	}
}

void CStuka::FreeMoving()
{
	auto targetPos = target->GetPosition();

	if (CMath::CalcDistance(targetPos, transform.position) < distanceTrigger)
	{
		state = StukaState::DetectedTarget;
	}
}

void CStuka::DetectedTarget()
{
	auto targetPos = target->GetPosition();
	if (CMath::CalcDistance(targetPos, transform.position) < distanceTrigger)
	{
		velocity = CMath::Normalize(targetPos - transform.position) * MOVE_SPEED;
	}
	else
	{
		state = StukaState::FreeMoving;
		if (velocity.x < 0) velocity.x = -MOVE_SPEED;
		else velocity.x = MOVE_SPEED;

		velocity.y = 0;
	}
}

void CStuka::Update(DWORD dt)
{
	if (target == nullptr)
	{
		state = StukaState::FreeMoving;
		if (velocity.x < 0) velocity.x = -MOVE_SPEED;
		else velocity.x = MOVE_SPEED;

		velocity.y = 0;
		return;
	}

	switch (state)
	{
	case StukaState::FreeMoving:
		FreeMoving();
		break;
	case StukaState::DetectedTarget:
		DetectedTarget();
		break;
	default:
		break;
	}

	OnDamagedUpdate();
}

void CStuka::Render()
{
	animations.at("Stuka")->Render(transform.position, 1, layer_index, 0, damagedColor[colorIndex]);
}