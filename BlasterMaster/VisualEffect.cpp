#include "VisualEffect.h"

CVisualEffect::CVisualEffect()
{
	layer_index = 2;
}

void CVisualEffect::Update(DWORD dt)
{
	if (animations.at(DEFAULT_ANI_NAME)->IsFinished() == true)
	{
		isEnabled = false;
		isDestroyed = true;
	}
}

void CVisualEffect::Render()
{
	animations.at(DEFAULT_ANI_NAME)->Render(transform.position, 1, layer_index);
}