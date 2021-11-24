#pragma once
#include "GameObject.h"

class CBallCarry : public CGameObject
{
private:
	void InitAnimations();
public:
	CBallCarry();
	~CBallCarry();

	void Update(DWORD dt);
	void Render();
};