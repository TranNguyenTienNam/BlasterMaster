#include "Thorn.h"
#include "BigJason.h"
#include "GX680.h"

void CThorn::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetBoxSize(Vector2(16.0f, 16.0f));
	collider->SetTrigger(true);
	colliders.push_back(collider);
}

CThorn::CThorn()
{
	InitColliders();

	tag = ObjectTag::HarmPlatform;
}

void CThorn::Update(DWORD dt)
{
}

void CThorn::Render()
{
}
