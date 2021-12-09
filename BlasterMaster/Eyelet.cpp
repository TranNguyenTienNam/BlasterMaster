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

	tag = ObjectTag::Eyelet;
}

CEyelet::~CEyelet()
{
}

void CEyelet::Update(DWORD dt)
{
	velocity.x = nx * 0.05f;

	float pi = 3.14f;
	angular += angularFrequency * dt;
	if (angular > 360.0f) angular = 0.0f;
	transform.position.y = 376.0f + angularAmplitude * sin(angular * pi / 180);

	DebugOut(L"eyelet %d %f %f\n", nx, transform.position.x, transform.position.y);
}

void CEyelet::Render()
{
	animations.at("Eyelet")->Render(transform.position, nx);
}