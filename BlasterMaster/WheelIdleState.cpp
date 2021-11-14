#include "WheelIdleState.h"
#include "InputHandler.h"
#include "CounterclockwiseState.h"
#include "ClockwiseState.h"
#include "Utils.h"

ISophiaState* CWheelIdleState::HandleInput(CSophia& sophia)
{
	DebugOut(L"Idle\n");
	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();
	if (inputHandler->IsKeyDown(DIK_RIGHT))
	{
		/*sophia.SetSpeed(Vector2(0.15f, 0.0f));*/
		return new CClockwiseState;
	}
	else if (inputHandler->IsKeyDown(DIK_LEFT))
	{
		/*sophia.SetSpeed(Vector2(-0.15f, 0.0f));*/
		return new CCounterclockwiseState;
	}
	else return nullptr;
}

void CWheelIdleState::Render(CSophia& sophia)
{
	auto anim = sophia.GetAnimations().at("Left-Wheel");
	anim->SetIsPaused(true);
	anim->Render(sophia.GetPosition(), 1);

	auto anim1 = sophia.GetAnimations().at("Right-Wheel");
	anim1->SetIsPaused(true);
	anim1->Render(sophia.GetPosition() + Vector2(10,0), 1);
}
