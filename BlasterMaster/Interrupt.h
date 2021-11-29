#pragma once
#include "GameObject.h"

class CInterrupt : public CGameObject
{
private:
	void InitAnimations();
public:
	CInterrupt();
	~CInterrupt();

	void Update(DWORD dt);
	void Render();
};