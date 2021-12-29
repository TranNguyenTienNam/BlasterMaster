#include "Eyelet.h"
#include "Animations.h"

void CEyelet::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Eyelet", animations->Get("ani-eyelet"));
}

void CEyelet::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	collider->SetTrigger(true);
	colliders.push_back(collider);
}

CEyelet::CEyelet()
{
	InitAnimations();
	InitColliders();
}

CEyelet::~CEyelet()
{
}

void CEyelet::Update(DWORD dt)
{
	velocity.x = nx * 0.05f;
	angular += angularFrequency * dt;
	transform.position.y = axisY + angularAmplitude * sin(angular * CMath::PI / 180);

	OnDamagedUpdate();
}

void CEyelet::Render()
{
	animations.at("Eyelet")->Render(transform.position, nx, layer_index, 0, damagedColor[colorIndex]);
}