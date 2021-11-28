#include "SophiaLeftWheel.h"
#include "Animations.h"
#include "WheelIdleState.h"
#include "ClockwiseState.h"
#include "CounterclockwiseState.h"

void CSophiaLeftWheel::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation(ANIM_LEFT_WHEEL, animations->Get("ani-sophia-left-wheel"));
}

CSophiaLeftWheel::CSophiaLeftWheel(CSophia* sophia)
{
	parent = sophia;
	InitAnimations();

}

CSophiaLeftWheel::~CSophiaLeftWheel()
{
}

void CSophiaLeftWheel::Update(DWORD dt)
{
	nx = parent->GetDirection();
}

void CSophiaLeftWheel::Render()
{
	auto stateWheel = parent->GetWheelState();
	auto anim = animations.at(ANIM_LEFT_WHEEL);
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
