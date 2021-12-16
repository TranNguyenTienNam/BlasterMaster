#include "SmallExplosion.h"
#include "Animations.h"

void CSmallExplosion::InitAnimations()
{
	auto animation_manager = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation(DEFAULT_ANI_NAME, animation_manager->Clone("ani-small-explosion"));
	animations.at(DEFAULT_ANI_NAME)->SetIsLooped(false);
}

CSmallExplosion::CSmallExplosion()
{
	InitAnimations();
}
