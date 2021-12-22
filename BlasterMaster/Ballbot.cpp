#include "Ballbot.h"
#include "Animations.h"
#include "Brick.h"

void CBallbot::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Ballbot", animations->Get("ani-ballbot"));
}

void CBallbot::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CBallbot::CBallbot()
{
	InitAnimations();
	InitColliders();

	acceleration.y = 0.0003f;
}

CBallbot::~CBallbot()
{
}

void CBallbot::SetState(BallbotState _state)
{
	state = _state;

	switch (_state)
	{
	case BallbotState::Sleeping:
	{
		velocity.x = 0;
		break;
	}
	case BallbotState::DetectedTarget:
	{
		auto targetPos = target->GetPosition();
		if (transform.position.x > targetPos.x)
		{
			nx = -1;
		}
		else
		{
			nx = 1;
		}
		velocity = Vector2(nx * MOVE_SPEED / 4, -MOVE_SPEED);
		break;
	}
	default:
		break;
	}
}

void CBallbot::Sleeping()
{
	auto targetPos = target->GetPosition();
	auto distance = CMath::CalcDistance(transform.position, targetPos);
	if (distance < distanceTrigger)
	{
		SetState(BallbotState::DetectedTarget);
	}
}

void CBallbot::DetectedTarget()
{
}

void CBallbot::Update(DWORD dt)
{
	velocity.y += acceleration.y * dt;

	if (target == nullptr) return;
	if (state == BallbotState::Sleeping) Sleeping();
}

void CBallbot::Render()
{
	animations.at("Ballbot")->Render(transform.position, nx, layer_index);
}

void CBallbot::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other))
	{
		if (collision->ny == -1)
		{
			SetState(BallbotState::Sleeping);
		}
	}
}
