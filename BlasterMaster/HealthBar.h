#pragma once
#include "GameObject.h"
#include "Playable.h"

class CHealthBar : public CGameObject
{
private:
	Vector2 offset;
	CCamera* mainCam;
	CPlayable* target;
	void InitSprites();
public:
	CHealthBar();
	void Update(DWORD dt);
	void Render();
};

