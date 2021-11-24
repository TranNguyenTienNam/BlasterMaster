#pragma once
#include "GameObject.h"

class CEyelet : public CGameObject
{
private:
	void InitAnimations();
public:
	CEyelet();
	~CEyelet();

	void Update(DWORD dt);
	void Render();
};