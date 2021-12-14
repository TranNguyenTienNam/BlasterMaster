#include "SmallExplosion.h"
#include "Animations.h"

void CSmallExplosion::InitAnimations()
{
	auto animation_manager = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Small-Explosion", animation_manager->Clone("ani-small-explosion"));
	animations.at("Small-Explosion")->SetIsLooped(false);
}

CSmallExplosion::CSmallExplosion()
{
	InitAnimations();
}

void CSmallExplosion::Update(DWORD dt)
{
	if (animations.at("Small-Explosion")->IsFinished() == true)
	{
		isEnabled = false;
		isDestroyed = true;
	}
}

void CSmallExplosion::Render()
{
	animations.at("Small-Explosion")->Render(transform.position, 1, layer_index);
}
