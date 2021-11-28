#include "ClockwiseState.h"
#include "Utils.h"

void CClockwiseState::Render()
{
	auto anim = owner->GetAnimations().at("Left-Wheel");
	anim->SetIsReversed(true);
	anim->SetIsPaused(false);

	anim = owner->GetAnimations().at("Right-Wheel");
	anim->SetIsReversed(true);
	anim->SetIsPaused(false);
}
