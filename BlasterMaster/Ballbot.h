#pragma once
#include "GameObject.h"

class CBallbot : public CGameObject
{
private:
	void InitAnimations();
public:
	CBallbot();
	~CBallbot();

	void Update(DWORD dt);
	void Render();
};