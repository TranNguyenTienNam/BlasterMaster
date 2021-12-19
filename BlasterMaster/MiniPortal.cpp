#include "MiniPortal.h"
#include "Jason.h"
#include "BigJason.h"

CMiniPortal::CMiniPortal(float w, float h, int scene_id)
{
	this->scene_id = scene_id;
	width = w;
	height = h;

	tag = ObjectTag::MiniPortal;

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

void CMiniPortal::OnOverlapped(CCollider2D* selfCollider, CGameObject* other)
{
	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();

	if (dynamic_cast<CJason*>(other) && inputHandler->OnKeyDown(DIK_DOWN))
	{
		CGame::GetInstance()->GetService<CScenes>()->SwitchScene(scene_id);
	}
}

void CMiniPortal::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	if (dynamic_cast<CBigJason*>(collision->obj))
	{
		CGame::GetInstance()->GetService<CScenes>()->SwitchScene(scene_id);
	}
}