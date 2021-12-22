#include "SophiaExplosion.h"
#include "Animations.h"

void CSophiaExplosion::InitAnimations()
{
	auto animation_manager = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation(DEFAULT_ANI_NAME, animation_manager->Clone("ani-sophia-explosion"));
	animations.at(DEFAULT_ANI_NAME)->SetIsLooped(false);
}

CSophiaExplosion::CSophiaExplosion()
{
	InitAnimations();
}

void CSophiaExplosion::Render()
{
	auto animation = animations.at(DEFAULT_ANI_NAME);
	if (animation->GetCurrentFrame() == 0)
		animation->Render(transform.position + Vector2(0.0f, -3.0f), 1, layer_index);
	else
	{
		animation->Render(transform.position, 1, layer_index);
	}
}
