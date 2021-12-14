#include "MiniPortal.h"
#include "Playable.h"

CMiniPortal::CMiniPortal(float w, float h, int scene_id)
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
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

void CMiniPortal::Update(DWORD dt)
{
}

void CMiniPortal::Render()
{
	colliders.at(0)->RenderBoundingBox();
}

void CMiniPortal::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CPlayable*>(other))
	{
		CGame::GetInstance()->GetService<CScenes>()->SwitchScene(scene_id);
	}
}
