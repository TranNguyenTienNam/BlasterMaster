#include "Portal.h"
#include "Playable.h"
#include "Sophia.h"

CPortal::CPortal(float w, float h, int scene_id)
{
	this->scene_id = scene_id;
	width = w;
	height = h;

	tag = ObjectTag::Portal;

	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(Vector2(0.0f, -height)); // TODO: ?????? Portal pos
	collider->SetBoxSize(Vector2(w, h));
	collider->SetTrigger(true);
	colliders.push_back(collider);
}

void CPortal::Update(DWORD dt)
{
}

void CPortal::Render()
{
	colliders.at(0)->RenderBoundingBox();
}
