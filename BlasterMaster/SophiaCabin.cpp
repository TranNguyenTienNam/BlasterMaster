#include "SophiaCabin.h"
#include "SophiaHorizontalState.h"
#include "SophiaUpward45State.h"
#include "SophiaUpwardState.h"

void CSophiaCabin::InitSprites()
{
	auto sprite_manager = CGame::GetInstance()->GetService<CSprites>();
	AddSprite(SPR_CABIN_00, sprite_manager->Get("spr-sophia-cabin"));
	AddSprite(SPR_CABIN_UPWARD, sprite_manager->Get("spr-sophia-cabin-45"));
}

CSophiaCabin::CSophiaCabin(CSophia* sophia)
{
	parent = sophia;
	InitSprites();

	// Init colliders
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(Vector2(16.0f, 8.0f));
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CSophiaCabin::~CSophiaCabin()
{
}

void CSophiaCabin::Update(DWORD dt)
{
	nx = parent->GetDirection();
}

void CSophiaCabin::Render()
{
	auto nx = parent->GetDirection();
	auto directionState = parent->GetDirectionState();
	if (dynamic_cast<CSophiaHorizontalState*>(directionState))
		sprites.at(SPR_CABIN_00)->Draw(transform.position + parent->GetPosition(), -nx, 255);
	else 
		sprites.at(SPR_CABIN_UPWARD)->Draw(transform.position + parent->GetPosition(), -nx, 255);
}