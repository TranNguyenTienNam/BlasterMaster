#pragma once
#include "VisualEffect.h"
class CSmallExplosion : public CVisualEffect
{
protected:
	void InitAnimations();
public:
	CSmallExplosion();
	virtual void Update(DWORD dt);
	virtual void Render();
};

