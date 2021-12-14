#include "BallCarry.h"
#include "Animations.h"
#include "Brick.h"
#include "BallBomb.h"

void CBallCarry::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("Sleeping", sprites->Get("spr-ballcarry-sleep"));
	AddSprite("Open", sprites->Get("spr-ballcarry-open"));
}

void CBallCarry::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("BallCarry", animations->Get("ani-ballcarry"));
}

void CBallCarry::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

void CBallCarry::DetectedTarget()
{
	auto targetPos = target->GetPosition();
	auto distance = CMath::CalcDistance(transform.position, targetPos);
	if (distance < distanceTrigger)
	{
		state = BallCarryState::DropingBomb;
	}
}

void CBallCarry::DroppingBomb()
{
	auto targetPos = target->GetPosition();

	if (didDroppedBomb == false)
	{
		for (int i = 0; i < maxBomb; i++)
		{
			auto bomb = Instantiate<CBallBomb>(transform.position);

			int sign = (rand() % 2 == 0) ? 1 : -1;

			float vx = CMath::Random(9, 12) * 0.001f * sign;
			float vy = CMath::Random(12, 15) * 0.01f;
			bomb->SetVelocity(Vector2(vx, vy));
		}

		didDroppedBomb = true;

		lastTimeDropBomb = GetTickCount();

		if (transform.position.x > targetPos.x) nx = 1;
		else nx = -1;
	}
}

void CBallCarry::PrepareRunning()
{
	DWORD now = GetTickCount();
	if (now - lastTimeDropBomb > dropBombDelay)
	{
		state = BallCarryState::RuningAway;
		velocity.x = nx * 0.05f;
	}
}

CBallCarry::CBallCarry()
{
	InitSprites();
	InitAnimations();
	InitColliders();

	state = BallCarryState::Sleeping;
}

CBallCarry::~CBallCarry()
{
}

void CBallCarry::Update(DWORD dt)
{
	velocity.y += -0.00026f * dt;

	switch (state)
	{
	case BallCarryState::Sleeping:
		DetectedTarget();
		break;
	case BallCarryState::DropingBomb:
		DroppingBomb();
		PrepareRunning();
		break;
	case BallCarryState::RuningAway:
		break;
	default:
		break;
	}
}

void CBallCarry::Render()
{
	switch (state)
	{
	case BallCarryState::Sleeping:
		sprites.at("Sleeping")->Draw(transform.position, nx, layer_index);
		break;
	case BallCarryState::DropingBomb:
		sprites.at("Open")->Draw(transform.position, nx, layer_index);
		break;
	case BallCarryState::RuningAway:
		animations.at("BallCarry")->Render(transform.position, nx, layer_index);
		break;
	default:
		break;
	}
}

void CBallCarry::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other) || dynamic_cast<CPlayable*>(other))
	{
		if (collision->nx != 0)
		{
			nx = collision->nx;
			velocity.x = nx * 0.05f;
		}
	}
}

void CBallCarry::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
}
