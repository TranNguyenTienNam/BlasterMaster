#pragma once
#include "GameObject.h"
class CThorn : public CGameObject
{
protected:
	const int damage = 1;
	void InitColliders();
public:
	CThorn();

	int GetDamage() { return -damage; }

	void Update(DWORD dt);
	void Render();
};

