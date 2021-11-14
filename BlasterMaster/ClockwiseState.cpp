#include "ClockwiseState.h"
#include "InputHandler.h"
#include "CounterclockwiseState.h"
#include "WheelIdleState.h"
#include "Utils.h"

ISophiaState* CClockwiseState::HandleInput(CSophia& sophia)
{
	DebugOut(L"Clockwise\n");
	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();
	if (inputHandler->IsKeyDown(DIK_LEFT))
	{
		/*sophia.SetSpeed(Vector2(-0.15f, 0.0f));*/
		return new CCounterclockwiseState;
	}
	else /*return nullptr;*/
	{
		DebugOut(L"????\n");
		/*sophia.SetSpeed(Vector2(0.0f, 0.0f));*/
		return new CWheelIdleState;
	}
}

void CClockwiseState::Render(CSophia& sophia)
{
	auto anim = sophia.GetAnimations().at("Left-Wheel");
	anim->SetIsReversed(true);
	anim->SetIsPaused(false);
	anim->Render(sophia.GetPosition(), 1);

	auto anim1 = sophia.GetAnimations().at("Right-Wheel");
	anim1->SetIsReversed(true);
	anim1->SetIsPaused(false);
	anim1->Render(sophia.GetPosition() + Vector2(10, 0), 1);
}
