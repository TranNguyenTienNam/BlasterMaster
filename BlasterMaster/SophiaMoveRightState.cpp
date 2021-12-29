#include "SophiaMoveRightState.h"

void CSophiaMoveRightState::Update(DWORD dt)
{
}

void CSophiaMoveRightState::Render()
{
	auto anim_left_wheel = owner->leftWheel->GetAnimations().at(ANIM_LEFT_WHEEL);
	auto anim_right_wheel = owner->rightWheel->GetAnimations().at(ANIM_RIGHT_WHEEL);

	anim_left_wheel->SetIsPaused(false);
	anim_left_wheel->SetIsReversed(true);
	anim_right_wheel->SetIsPaused(false);
	anim_right_wheel->SetIsReversed(true);
}