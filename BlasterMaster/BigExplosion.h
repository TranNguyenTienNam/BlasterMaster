#pragma once
#include "VisualEffect.h"
class CBigExplosion : public CVisualEffect
{
protected:
	void InitAnimations();
public:
	CBigExplosion();
	virtual void Update(DWORD dt);
	virtual void Render();
};

