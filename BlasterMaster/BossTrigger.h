#pragma once
#include "GameObject.h"

class CBossTrigger : public CGameObject
{
	const DWORD switchingDuration = 4500;
	DWORD timeEnteringRoom;

	int scene_id;			// target scene to switch to 

	int width;
	int height;
public:
	CBossTrigger(float w, float h, int scene_id);
	int GetSceneId() { return scene_id; }

	void Update(DWORD dt);
	void Render();

	void OnOverlapped(CCollider2D* selfCollider, CGameObject* object);
};

