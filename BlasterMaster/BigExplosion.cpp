#include "BigExplosion.h"
#include "Animations.h"

void CBigExplosion::InitAnimations()
{
	auto animation_manager = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Big-Explosion", animation_manager->Clone("ani-big-explosion"));
	animations.at("Big-Explosion")->SetIsLooped(false);
}

CBigExplosion::CBigExplosion()
{
	InitAnimations();
}

void CBigExplosion::Update(DWORD dt)
{
	if (animations.at("Big-Explosion")->IsFinished() == true)
	{
		isEnabled = false;
		isDestroyed = true;
	}
}

void CBigExplosion::Render()
{
	animations.at("Big-Explosion")->Render(transform.position, 1, layer_index);
}
