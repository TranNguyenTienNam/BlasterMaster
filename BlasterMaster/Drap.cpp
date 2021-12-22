#include "Drap.h"
#include "Animations.h"
#include "Utils.h"
#include "Transform.h"
#include "Brick.h"

void CDrap::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Clockwise", animations->Get("ani-drap"));
}

void CDrap::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CDrap::CDrap()
{
	InitAnimations();
	InitColliders();

	scrollingMap = false;
	monitorSide = -1;
}

CDrap::~CDrap()
{
}

void CDrap::Update(DWORD dt)
{
	if (target == nullptr) return;

	auto targetPos = target->GetPosition();
	if (abs(targetPos.y - transform.position.y) < rangeTrigger)
	{
		velocity.x = monitorSide * MOVE_SPEED;
	}
}

void CDrap::Render()
{
	Vector2 pos = transform.position;
	animations.at("Clockwise")->Render(pos, nx, layer_index);
}

void CDrap::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other))
	{
		monitorSide = collision->nx;
	}
}
