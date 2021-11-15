#include "Sophia.h"
#include "Animations.h"
#include "WheelIdleState.h"
#include "ClockwiseState.h"
#include "CounterclockwiseState.h"
#include "SophiaIdleState.h"
#include "SophiaUpward45State.h"
#include "SophiaUpwardState.h"
#include "InputHandler.h"
#include "Utils.h"

CSophia::CSophia()
{
	InitAnimation();
	stateWheel = new CWheelIdleState;
	stateDirection = new CSophiaIdleState;
}

CSophia::~CSophia()
{	
}

void CSophia::InitAnimation()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Left-Wheel", animations->Get("ani-sophia-left-wheel"));
	AddAnimation("Right-Wheel", animations->Get("ani-sophia-right-wheel"));

	sprMiddle = CGame::GetInstance()->GetService<CSprites>()->Get("spr-sophia-middle");
}

void CSophia::Update(DWORD dt)
{
	transform.position.x += velocity.x * dt;
	transform.position.y += velocity.y * dt;
	if (transform.position.y < 50) transform.position.y = 50;

	velocity.y -= 0.0026f * dt;

	// TODO: Move all changes of states into UpdateState() and the others into Update() of class State
	// TODO: Maybe combine Wheel State with Physical State 
	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();
	if (inputHandler->IsKeyDown(DIK_RIGHT))
	{
		velocity.x = 0.15f;
		nx = 1;
		stateWheel = new CClockwiseState;
	}
	else if (inputHandler->IsKeyDown(DIK_LEFT))
	{
		velocity.x = -0.15f;
		nx = -1;
		stateWheel = new CCounterclockwiseState;
	}
	else
	{
		velocity.x = 0.0f;
		stateWheel = new CWheelIdleState;
	}

	if (inputHandler->IsKeyDown(DIK_UP))
	{
		DWORD now = GetTickCount();
		lastTime2 = now;
		if (dynamic_cast<CSophiaIdleState*>(stateDirection))
		{
			stateDirection = new CSophiaUpward45State;
		}
		else if (now - lastTime > 200 && dynamic_cast<CSophiaUpward45State*>(stateDirection))
		{
			stateDirection = new CSophiaUpwardState;
		}
	}
	else
	{
		lastTime = GetTickCount();
		DWORD now = lastTime;
		if (dynamic_cast<CSophiaUpwardState*>(stateDirection))
		{
			stateDirection = new CSophiaUpward45State;
		}
		else if (now - lastTime2 > 100 && dynamic_cast<CSophiaUpward45State*>(stateDirection))
		{
			stateDirection = new CSophiaIdleState;
		}
	}

	stateDirection->Update(dt, *this, nx);

	if (inputHandler->OnKeyDown(DIK_X))
	{
		if (transform.position.y == 50) velocity.y = 0.7f;

		DWORD now = GetTickCount();

		if (inputHandler->IsKeyDown(DIK_X))
		{

		}
	}
}

void CSophia::Render()
{
	stateWheel->Render(*this);
	animations.at("Left-Wheel")->Render(transform.position + posLeftWheel, 1);
	animations.at("Right-Wheel")->Render(transform.position + posRightWheel, 1);
	sprMiddle->Draw(transform.position + posMiddle, nx, 255);
	sprCabin->Draw(transform.position + posCabin, nx, 255);
	sprGun->Draw(transform.position + posGun, nx, 255);
}
