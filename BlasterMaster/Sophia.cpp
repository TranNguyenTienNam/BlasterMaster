#include "Sophia.h"
#include "Animations.h"
#include "WheelIdleState.h"
#include "ClockwiseState.h"
#include "CounterclockwiseState.h"
#include "Utils.h"
#include "InputHandler.h"

CSophia::CSophia()
{
	InitAnimation();
	stateWheel = new CWheelIdleState;
}

CSophia::~CSophia()
{	
}

void CSophia::InitAnimation()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Left-Wheel", animations->Get("ani-sophia-left-wheel"));
	AddAnimation("Right-Wheel", animations->Get("ani-sophia-right-wheel"));
}

void CSophia::Update(DWORD dt)
{
	transform.position.x += velocity.x * dt;
	transform.position.y += velocity.y * dt;

	DebugOut(L"/////////////////////////////Update\n");

	// TODO: Move all changes of states into UpdateState() and the others into Update() of class State
	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();
	if (inputHandler->IsKeyDown(DIK_RIGHT))
	{
		velocity.x = 0.15f;
		stateWheel = new CClockwiseState;
	}
	else if (inputHandler->IsKeyDown(DIK_LEFT))
	{
		velocity.x = -0.15f;
		stateWheel = new CCounterclockwiseState;
	}
	else
	{
		velocity.x = 0.0f;
		stateWheel = new CWheelIdleState;
	}

	/*ISophiaState* _state = state->HandleInput(*this);
	if (_state != nullptr)
	{
		delete state;
		state = _state;
	}*/
}

void CSophia::Render()
{
	stateWheel->Render(*this);
	/*animations.at("Wheel")->Render(transform.position, 1);*/
}
