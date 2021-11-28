#pragma once
#include "Sophia.h"

#define ANIM_RIGHT_WHEEL "Right-Wheel"

class CSophiaRightWheel : public CGameObject
{
protected:
	CSophia* parent;
	void InitAnimations();
public:
	CSophiaRightWheel(CSophia* sophia);
	~CSophiaRightWheel();
	virtual void Update(DWORD dt);
	virtual void Render();
};