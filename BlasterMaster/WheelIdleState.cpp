#include "WheelIdleState.h"
#include "Utils.h"

void CWheelIdleState::Render()
{
	auto animations = owner->GetAnimations();
	animations.at("Left-Wheel")->SetIsPaused(true);
	animations.at("Right-Wheel")->SetIsPaused(true);
}
