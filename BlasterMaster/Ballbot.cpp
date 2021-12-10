#include "Ballbot.h"
#include "Animations.h"

void CBallbot::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Ballbot", animations->Get("ani-ballbot"));
}

void CBallbot::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	colliders.push_back(collider);
}

CBallbot::CBallbot()
{
	InitAnimations();
	InitColliders();
}

CBallbot::~CBallbot()
{
}

void CBallbot::Update(DWORD dt)
{
}

void CBallbot::Render()
{
	animations.at("Ballbot")->Render(transform.position, nx, layer_index);
}