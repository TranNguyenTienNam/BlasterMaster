#pragma once

#include "GameObject.h"

class CGX680 : public CGameObject
{
private:
	void InitAnimations();
public:
	CGX680();
	~CGX680();

	void Update(DWORD dt);
	void Render();
};