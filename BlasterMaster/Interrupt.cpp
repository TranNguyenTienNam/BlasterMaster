#include "Interrupt.h"

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

CInterrupt::CInterrupt()
{
	InitAnimations();
	InitColliders();
}

CInterrupt::~CInterrupt()
{
}

void CInterrupt::Update(DWORD dt)
{
}

void CInterrupt::Render()
{
	sprites.at("Close")->Draw(transform.position, 1);
}
