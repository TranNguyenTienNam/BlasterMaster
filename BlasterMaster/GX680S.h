#pragma once
#include "GX680.h"

class CGX680S : public CGameObject
{
private:
	void InitAnimations();
public:
	CGX680S();
	~CGX680S();

	void Update(DWORD dt);
	void Render();
};