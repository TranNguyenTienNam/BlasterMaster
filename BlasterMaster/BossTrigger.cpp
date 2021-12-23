#include "BossTrigger.h"
#include "BigJason.h"
#include "Portal.h"

CBossTrigger::CBossTrigger(float w, float h, int scene_id)
{
	this->scene_id = scene_id;
	width = w;
	height = h;

	tag = ObjectTag::BossTrigger;

	transform.position = Vector2(0.0f, -height);

	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(Vector2(w, h));
	collider->SetTrigger(true);
	collider->SetDynamic(true);
	colliders.push_back(collider);

	timeEnteringRoom = GetTickCount();
}

void CBossTrigger::Update(DWORD dt)
{
}

void CBossTrigger::Render()
{
}

void CBossTrigger::OnOverlapped(CCollider2D* selfCollider, CGameObject* object)
{
	if (dynamic_cast<CBigJason*>(object))
	{
		auto currentScene = (CPlayScene*)CGame::GetInstance()->GetService<CScenes>()->GetCurrentScene();
		for (auto portal : currentScene->GetPortalList())
		{
			if (portal.first == scene_id - 2)
			{
				auto coPortal = ((CPortal*)portal.second)->GetColliders().at(0);
				coPortal->SetTrigger(false);
				coPortal->SetDynamic(false);
				break;
			}
		}

		DWORD now = GetTickCount();
		if (now - timeEnteringRoom > switchingDuration)
		{
			CGame::GetInstance()->GetService<CScenes>()->SwitchScene(scene_id);
		}
	}
}
