#include "BallCarry.h"
#include "Animations.h"
#include "Brick.h"

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

CBallCarry::CBallCarry()
{
	InitAnimations();
	InitColliders();

	tag = ObjectsTag::Enemy;
}

CBallCarry::~CBallCarry()
{
}

void CBallCarry::Update(DWORD dt)
{
	velocity.y += -0.00026f * dt;
}

void CBallCarry::Render()
{
	animations.at("BallCarry")->Render(transform.position, nx);
}

void CBallCarry::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other))
	{
		if (collision->nx != 0)
		{
			nx = -nx;
			velocity.x = nx * 0.05f;
		}
		else if (collision->ny != 0)
		{
			velocity.x = nx * 0.05f;
		}
	}
}

void CBallCarry::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
}
