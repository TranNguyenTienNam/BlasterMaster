#pragma once
#include "GameObject.h"
class CVisualEffect : public CGameObject
{
public:
	CVisualEffect();
	virtual void Update(DWORD dt);
	virtual void Render();
};

