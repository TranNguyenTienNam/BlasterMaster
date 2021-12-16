#pragma once
#include "GameObject.h"

/*
	Object that triggers scene switching
*/

class CPortal : public CGameObject
{
	Vector2 translation;	// translation position of game object and background when switch section
	int scene_id;			// target scene to switch to 

	int width;
	int height;
public:
	CPortal(float w, float h, int scene_id);
	void SetTranslation(Vector2 trans) { this->translation = trans; }
	void Update(DWORD dt);
	void Render();
	int GetSceneId() { return scene_id; }

	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};