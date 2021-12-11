#include "Portal.h"

CPortal::CPortal(float l, float t, float w, float h, int scene_id)
{
	this->scene_id = scene_id;
	transform.position.x = l;
	transform.position.y = t;
	width = w;
	height = h;

	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(Vector2(w, h));
	colliders.push_back(collider);
}

void CPortal::Update(DWORD dt)
{
}