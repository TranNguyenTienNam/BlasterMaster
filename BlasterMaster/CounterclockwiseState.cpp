#include "CounterclockwiseState.h"
#include "Utils.h"

void CCounterclockwiseState::Render()
{
	auto anim = owner->GetAnimations().at("Left-Wheel");
	anim->SetIsReversed(false);
	anim->SetIsPaused(false);

	anim = owner->GetAnimations().at("Right-Wheel");
	anim->SetIsReversed(false);
	anim->SetIsPaused(false);
}
