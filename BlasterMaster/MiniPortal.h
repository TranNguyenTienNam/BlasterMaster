#pragma once
#include "GameObject.h"
class CMiniPortal : public CGameObject
{
	int scene_id;			// target scene to switch to 

	int width;
	int height;
public:
	CMiniPortal(float w, float h, int scene_id);
	void Update(DWORD dt);
	void Render();
	int GetSceneId() { return scene_id; }

	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

