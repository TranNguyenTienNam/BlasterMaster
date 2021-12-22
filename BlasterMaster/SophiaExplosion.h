#pragma once
#include "VisualEffect.h"

class CSophiaExplosion : public CVisualEffect
{
protected:
	void InitAnimations();
public:
	CSophiaExplosion();
	void Render() override;
};

