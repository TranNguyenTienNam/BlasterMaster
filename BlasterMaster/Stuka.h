#pragma once
#include "GameObject.h"

class CStuka : public CGameObject
{
private:
	void InitAnimations();
public:
	CStuka();
	~CStuka();

	void Update(DWORD dt);
	void Render();
};