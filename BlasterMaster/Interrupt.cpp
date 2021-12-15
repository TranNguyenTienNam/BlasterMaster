#include "Interrupt.h"
#include "Sprites.h"
#include "NeowormLarva.h"

void CInterrupt::InitAnimations()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("Close", sprites->Get("spr-interrupt-close"));
	AddSprite("Open", sprites->Get("spr-interrupt-open"));
}

void CInterrupt::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	colliders.push_back(collider);
}

void CInterrupt::DetectedTarger()
{
	auto targetPos = target->GetPosition();
	auto distance = CMath::CalcDistance(transform.position, targetPos);
	if (targetPos.x < transform.position.x + rangeTrigger &&
		targetPos.x > transform.position.x - rangeTrigger &&
		targetPos.y < transform.position.y && distance < distanceTrigger)
	{
		SetState(InterruptState::Openning);
		DroppingNeowormLarva();
	}
}

void CInterrupt::DroppingNeowormLarva()
{
	Instantiate<CNeowormLarva>(transform.position + Vector2(-1.0f, -5.0f));
}

CInterrupt::CInterrupt()
{
	InitAnimations();
	InitColliders();

	SetState(InterruptState::Sleeping);
}

CInterrupt::~CInterrupt()
{
}

void CInterrupt::SetState(InterruptState _state)
{
	state = _state;

	switch (state)
	{
	case InterruptState::Sleeping:
		sprite = sprites.at("Close");
		break;
	case InterruptState::Openning:
		sprite = sprites.at("Open");
		break;
	default:
		break;
	}
}

void CInterrupt::Update(DWORD dt)
{
	if (state == InterruptState::Sleeping)
	{
		DetectedTarger();
	}
}

void CInterrupt::Render()
{
	sprite->Draw(transform.position, nx, layer_index);
}
