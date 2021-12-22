#include "GX680.h"
#include "Animations.h"
#include "Brick.h"

void CGX680::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("GX680", animations->Get("ani-gx680"));
}

void CGX680::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

void CGX680::FreeMotion()
{
	auto targetPos = target->GetPosition();

	DWORD now = GetTickCount();
	if (now - lastTimeChangeDirection > changeDirectionDelay)
	{
		int signX = (rand() % 2 == 0) ? 1 : -1;
		int signY = (rand() % 2 == 0) ? 1 : -1;

		velocity = CMath::Normalize(Vector2(rand() * signX, rand() * signY)) * MOVE_SPEED;
		lastTimeChangeDirection = now;
	}

	if (CMath::CalcDistance(targetPos, transform.position) < distanceTrigger)
	{
		state = GX680State::DetectedTarget;
	}
}

void CGX680::DetectedTarget()
{
	auto targetPos = target->GetPosition();
	velocity = CMath::Normalize(targetPos - transform.position) * MOVE_SPEED;

	if (CMath::CalcDistance(targetPos, transform.position) > distanceTrigger)
	{
		state = GX680State::FreeMotion;
	}
}

CGX680::CGX680()
{
	InitAnimations();
	InitColliders();

	scrollingMap = false;
	state = GX680State::FreeMotion;
	lastTimeChangeDirection = GetTickCount();
}

CGX680::~CGX680()
{
}

void CGX680::Update(DWORD dt)
{
	switch (state)
	{
	case GX680State::FreeMotion:
		FreeMotion();
		break;
	case GX680State::DetectedTarget:
		DetectedTarget();
		break;
	default:
		break;
	}
}

void CGX680::Render()
{
	animations.at("GX680")->Render(transform.position, 1, layer_index);
}

void CGX680::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other))
	{
		int signX = 0;
		if (collision->nx != 0)
		{
			signX = collision->nx;
		}
		else
		{
			signX = (rand() % 2 == 0) ? 1 : -1;
		}

		int signY = 0;
		if (collision->ny != 0)
		{
			signY = collision->ny;
		}
		else
		{
			signY = (rand() % 2 == 0) ? 1 : -1;
		}

		velocity = CMath::Normalize(Vector2(rand() * signX, rand() * signY)) * MOVE_SPEED;
	}
}
