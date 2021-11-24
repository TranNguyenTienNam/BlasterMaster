#pragma once
#include "GameObject.h"

class CNeoworm : public CGameObject
{
private:
	void InitAnimations();
public:
	CNeoworm();
	~CNeoworm();

	void Update(DWORD dt);
	void Render();
};