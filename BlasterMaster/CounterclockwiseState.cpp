#include "CounterclockwiseState.h"
#include "InputHandler.h"
#include "ClockwiseState.h"
#include "WheelIdleState.h"
#include "Utils.h"

ISophiaState* CCounterclockwiseState::HandleInput(CSophia& sophia)
{
	DebugOut(L"Counter clockwise\n");
	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();
	if (inputHandler->IsKeyDown(DIK_RIGHT))
	{
		/*sophia.SetSpeed(Vector2(0.15f, 0.0f));*/
		return new CClockwiseState;
	}
	else
	{
		/*sophia.SetSpeed(Vector2(0.0f, 0.0f));*/
		return new CWheelIdleState;
	}
}

void CCounterclockwiseState::Render(CSophia& sophia)
{
	auto anim = sophia.GetAnimations().at("Left-Wheel");
	anim->SetIsReversed(false);
	anim->SetIsPaused(false);
	anim->Render(sophia.GetPosition(), 1);

	auto anim1 = sophia.GetAnimations().at("Right-Wheel");
	anim1->SetIsReversed(false);
	anim1->SetIsPaused(false);
	anim1->Render(sophia.GetPosition() + Vector2(10, 0), 1);
}
