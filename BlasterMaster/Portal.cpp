#include "Portal.h"

CPortal::CPortal(float w, float h, int scene_id)
{
	this->scene_id = scene_id;
	width = w;
	height = h;

	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(Vector2(0.0f, -32.0f)); // TODO: ?????? Portal pos
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
