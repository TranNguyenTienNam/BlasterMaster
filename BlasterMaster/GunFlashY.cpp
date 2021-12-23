#include "GunFlashY.h"
#include "Animations.h"

void CGunFlashY::InitAnimations()
{
	auto animation_manager = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation(DEFAULT_ANI_NAME, animation_manager->Clone("ani-gun-flash-axisy"));
	animations.at(DEFAULT_ANI_NAME)->SetIsLooped(false);
}

CGunFlashY::CGunFlashY()
{
	InitAnimations();
}

void CGunFlashY::Render()
{
	animations.at(DEFAULT_ANI_NAME)->Render(transform.position, nx, layer_index, angleZ);
}
