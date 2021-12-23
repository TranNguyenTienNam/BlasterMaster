#pragma once
#include "VisualEffect.h"
class CGunFlashY : public CVisualEffect
{
protected:
	void InitAnimations();
public:
	CGunFlashY();
	void Render();
};

