#include "Sophia.h"
#include "Animations.h"
#include "WheelIdleState.h"
#include "ClockwiseState.h"
#include "CounterclockwiseState.h"
#include "SophiaHorizontalState.h"
#include "SophiaUpward45State.h"
#include "SophiaUpwardState.h"
#include "InputHandler.h"
#include "Utils.h"
#include "Brick.h"
#include "Jason.h"

CSophia::CSophia()
{
	InitAnimations();
	leftWheel = new CSophiaLeftWheel(this);
	rightWheel = new CSophiaRightWheel(this);
	middle = new CSophiaMiddle(this);
	cabin = new CSophiaCabin(this);
	gun = new CSophiaGun(this);

	stateWheel = new CWheelIdleState();
	stateDirection = new CSophiaHorizontalState;
	stateWheel->SetOwner(this);
	stateDirection->SetOwner(this);

	// Init collider
	stateDirection->UpdateColliders();

	// Player's settings
	controllable = true;
	sophia = this;
}

CSophia::~CSophia()
{
}

void CSophia::Update(DWORD dt)
{
#pragma region Physics Update
	/*velocity.x += acceleration.x * dt;*/
	velocity.y += -0.0026f * dt; // TODO: Need to adjust gravity
	/*if (abs(velocity.y) > 0.02) velocity.y = -0.02;*/
#pragma endregion

#pragma region Input Handling
	if (controllable == false)
	{
		if (dynamic_cast<CWheelIdleState*>(stateWheel) == nullptr) 
			stateWheel = new CWheelIdleState;
		if (dynamic_cast<CSophiaHorizontalState*>(stateDirection) == nullptr) 
			stateDirection = new CSophiaHorizontalState;
	}
	else
	{
		// TODO: Move all changes of states into UpdateState() and the others into Update() of class State
		// TODO: Maybe combine Wheel State with Physical State 
		auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();

		if (inputHandler->IsKeyDown(PlayerKeySet::MOVE_RIGHT_KEY))
		{
			velocity.x = 0.15f;
			/*acceleration.x = 0.0002f;*/
			nx = 1;
			stateWheel = new CClockwiseState;
		}
		else if (inputHandler->IsKeyDown(PlayerKeySet::MOVE_LEFT_KEY))
		{
			velocity.x = -0.15f;
			/*acceleration.x = -0.0002f;*/
			nx = -1;
			stateWheel = new CCounterclockwiseState;
		}
		else
		{
			velocity.x = 0.0f;
			/*acceleration.x = 0.0f;*/
			stateWheel = new CWheelIdleState;
		}

		// Update gun's direction
		if (dynamic_cast<CSophiaUpwardState*>(stateDirection)) lastTimeToLowerGun = GetTickCount();

		if (inputHandler->IsKeyDown(PlayerKeySet::SOPHIA_UPWARD_KEY))
		{
			DWORD now = GetTickCount();
			if (dynamic_cast<CSophiaHorizontalState*>(stateDirection))
			{
				stateDirection = new CSophiaUpward45State;
			}
			else if (now - lastTimeToLiftGun > 200 && dynamic_cast<CSophiaUpward45State*>(stateDirection))
			{
				stateDirection = new CSophiaUpwardState;
			}
		}
		else
		{
			lastTimeToLiftGun = GetTickCount();
			DWORD now = lastTimeToLiftGun;
			if (dynamic_cast<CSophiaUpwardState*>(stateDirection))
			{
				stateDirection = new CSophiaUpward45State;
			}
			else if (now - lastTimeToLowerGun > 200 && dynamic_cast<CSophiaUpward45State*>(stateDirection))
			{
				stateDirection = new CSophiaHorizontalState;
			}
		}

		if (inputHandler->OnKeyDown(PlayerKeySet::JUMPING_KEY) && onGround == true)
		{
			onGround = false;
			velocity.y = 0.7f; // TODO: Jump speed?

			DWORD now = GetTickCount();

			if (inputHandler->IsKeyDown(PlayerKeySet::JUMPING_KEY))
			{

			}
		}
	}
#pragma endregion

	// State Update
	stateDirection->Update(dt);
	stateDirection->UpdateColliders();

	// Part Update
	leftWheel->Update(dt);
	rightWheel->Update(dt);
	middle->Update(dt);
	cabin->Update(dt);
	gun->Update(dt);
}

void CSophia::Render()
{
	leftWheel->Render();
	rightWheel->Render();
	middle->Render();
	cabin->Render();
	gun->Render();
}

void CSophia::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	if (dynamic_cast<CBrick*>(collision->obj))
	{
		if (onGround == false && collision->ny == 1) onGround = true;
	}
}

void CSophia::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
}
