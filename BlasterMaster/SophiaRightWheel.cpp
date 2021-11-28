#include "SophiaRightWheel.h"
#include "Animations.h"
#include "WheelIdleState.h"
#include "ClockwiseState.h"
#include "CounterclockwiseState.h"

void CSophiaRightWheel::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation(ANIM_RIGHT_WHEEL, animations->Get("ani-sophia-right-wheel"));
}

CSophiaRightWheel::CSophiaRightWheel(CSophia* sophia)
{
	parent = sophia;
	InitAnimations();
}

CSophiaRightWheel::~CSophiaRightWheel()
{
}

void CSophiaRightWheel::Update(DWORD dt)
{
	nx = parent->GetDirection();
}

void CSophiaRightWheel::Render()
{
	auto stateWheel = parent->GetWheelState();
	auto anim = animations.at(ANIM_RIGHT_WHEEL);
	if (dynamic_cast<CWheelIdleState*>(stateWheel))
	{
		anim->SetIsPaused(true);
	}
	else
	{
		anim->SetIsPaused(false);
		if (dynamic_cast<CClockwiseState*>(stateWheel))
			anim->SetIsReversed(true);
		else if (dynamic_cast<CCounterclockwiseState*>(stateWheel))
			anim->SetIsReversed(false);
	}

	anim->Render(transform.position + parent->GetPosition(), -1);
}