#pragma once
#include "Sophia.h"

#define ANIM_LEFT_WHEEL "Left-Wheel"

class CSophiaLeftWheel : public CGameObject
{
protected:
	CSophia* parent;
	void InitAnimations();
public:
	CSophiaLeftWheel(CSophia* sophia);
	~CSophiaLeftWheel();
	virtual void Update(DWORD dt);
	virtual void Render();
};