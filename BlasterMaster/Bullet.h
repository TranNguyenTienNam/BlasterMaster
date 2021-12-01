#pragma once
#include "GameObject.h"

class CBullet : public CGameObject
{
public:
	CBullet();
	~CBullet();

	virtual void Update(DWORD dt);
	virtual void Render();
};

