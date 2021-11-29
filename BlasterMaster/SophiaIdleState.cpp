#include "SophiaIdleState.h"

void CSophiaIdleState::Update(DWORD dt)
{

}

void CSophiaIdleState::Render()
{
	auto anim_left_wheel = owner->leftWheel->GetAnimations().at(ANIM_LEFT_WHEEL);
	auto anim_right_wheel = owner->rightWheel->GetAnimations().at(ANIM_RIGHT_WHEEL);

	anim_left_wheel->SetIsPaused(true);
	anim_right_wheel->SetIsPaused(true);
}
