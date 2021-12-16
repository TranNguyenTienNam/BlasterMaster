#include "RandomExplosion.h"
#include "Animations.h"

void CRandomExplosion::InitAnimations()
{
	auto animation_manager = CGame::GetInstance()->GetService<CAnimations>();
	
	if (rand() % 2 == 0)
	{
		AddAnimation(DEFAULT_ANI_NAME, animation_manager->Clone("ani-big-bullet-explosion"));
	}
	else AddAnimation(DEFAULT_ANI_NAME, animation_manager->Clone("ani-small-bullet-explosion"));

	animations.at(DEFAULT_ANI_NAME)->SetIsLooped(false);
}

CRandomExplosion::CRandomExplosion()
{
	InitAnimations();
}
