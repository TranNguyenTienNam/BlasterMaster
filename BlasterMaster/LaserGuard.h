#pragma once
#include "GameObject.h"

class CLaserGuard : public CGameObject
{
private:
	void InitAnimations();
public:
	CLaserGuard();
	~CLaserGuard();

	void Update(DWORD dt);
	void Render();
};