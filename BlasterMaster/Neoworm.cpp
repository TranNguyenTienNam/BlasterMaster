#include "Neoworm.h"
#include "Animations.h"

void CNeoworm::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("Larva", sprites->Get("spr-neoworm-larva"));
}

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
	InitSprites();
	InitAnimations();
	InitColliders();
}

CNeoworm::~CNeoworm()
{
}

void CNeoworm::Update(DWORD dt)
{
}

void CNeoworm::Render()
{
	animations.at("Move")->Render(transform.position, nx, layer_index);
}
