#include "GunFlashX.h"
#include "Animations.h"

void CGunFlashX::InitAnimations()
{
	auto animation_manager = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation(DEFAULT_ANI_NAME, animation_manager->Clone("ani-gun-flash-axisx"));
	animations.at(DEFAULT_ANI_NAME)->SetIsLooped(false);
}

CGunFlashX::CGunFlashX()
{
	InitAnimations();
}
