#include "Neoworm.h"
#include "Animations.h"
#include "Brick.h"

void CNeoworm::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Move", animations->Get("ani-neoworm"));
}

void CNeoworm::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CNeoworm::CNeoworm()
{
	InitAnimations();
	InitColliders();
}

CNeoworm::~CNeoworm()
{
}

void CNeoworm::Update(DWORD dt)
{
	velocity.y += -0.00026f * dt;
}

void CNeoworm::Render()
{
	animations.at("Move")->Render(transform.position, nx, layer_index);
}

void CNeoworm::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other) && collision->nx != 0)
	{
		nx = collision->nx;
		velocity.x = nx * MOVE_SPEED;
	}
}
