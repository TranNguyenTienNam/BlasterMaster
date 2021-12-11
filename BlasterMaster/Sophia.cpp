#include "Sophia.h"
#include "Animations.h"
#include "SophiaIdleState.h"
#include "SophiaMoveLeftState.h"
#include "SophiaMoveRightState.h"
#include "SophiaHorizontalState.h"
#include "SophiaUpward45State.h"
#include "SophiaUpwardState.h"
#include "InputHandler.h"
#include "Utils.h"
#include "Brick.h"
#include "Jason.h"
#include "Camera.h"
#include "Scenes.h"
#include "Enemy.h"
#include "PlayScene.h"

void CSophia::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(OFFSET_SOPHIA_IDLE);
	collider->SetBoxSize(BOX_SOPHIA_IDLE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CSophia::CSophia()
{
	InitColliders();

	leftWheel = new CSophiaLeftWheel(this);
	rightWheel = new CSophiaRightWheel(this);
	middle = new CSophiaMiddle(this);
	cabin = new CSophiaCabin(this);
	gun = new CSophiaGun(this);

	stateAction = new CSophiaIdleState;
	stateDirection = new CSophiaHorizontalState;
	stateAction->SetOwner(this);
	stateDirection->SetOwner(this);

	// Player's settings
	tag = ObjectTag::Player;
	controllable = true;
	untouchable = false;
	sophia = this;
}

void CSophia::Update(DWORD dt)
{
	DebugOut(L"sophia pos %f %f\n", transform.position.x, transform.position.y);

	velocity.x += acceleration.x * dt;
	if (colliders.at(0)->IsDynamic() == true)
		velocity.y += GRAVITY * dt;
	/*if (abs(velocity.y) > 0.02) velocity.y = -0.02;*/

#pragma region State Transition
	if (controllable == false)
	{
		if (dynamic_cast<CSophiaIdleState*>(stateAction) == nullptr)
			stateAction = new CSophiaIdleState;
		if (dynamic_cast<CSophiaHorizontalState*>(stateDirection) == nullptr) 
			stateDirection = new CSophiaHorizontalState;
	}
	else
	{
		// TODO: Move all changes of states into UpdateState() and the others into Update() of class State
		auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();

		if (inputHandler->IsKeyDown(PlayerKeySet::MOVE_RIGHT_KEY))
		{
			acceleration.x = MOVE_ACCELERATION;
			nx = 1;
			stateAction = new CSophiaMoveLeftState;
		}
		else if (inputHandler->IsKeyDown(PlayerKeySet::MOVE_LEFT_KEY))
		{
			acceleration.x = -MOVE_ACCELERATION;
			nx = -1;
			stateAction = new CSophiaMoveRightState;
		}
		else
		{
			velocity.x = 0.0f;
			acceleration.x = 0.0f;
			stateAction = new CSophiaIdleState;
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
			velocity.y = JUMP_SPEED;

			DWORD now = GetTickCount();

			if (inputHandler->IsKeyDown(PlayerKeySet::JUMPING_KEY))
			{

			}
		}

		if (inputHandler->OnKeyDown(PlayerKeySet::SHOOTING_KEY))
		{
			stateDirection->Shooting();
		}

		if (inputHandler->OnKeyDown(PlayerKeySet::SWITCH_CHARACTER_KEY) &&
			GetTickCount() - lastTimeSwitch > switchDelay)
		{
			lastTimeSwitch = GetTickCount();
			// Sophia is not controllable, collider is trigger, animation is idle
			controllable = false;
			velocity = VectorZero();
			for (auto co : colliders)
			{
				co->SetTrigger(true);
				co->SetDynamic(false);
			}

			// Enable Jason, set jason's position, state is jumping
			auto game = CGame::GetInstance();
			((CPlayScene*)game->GetService<CScenes>()->GetCurrentScene())->SetPlayer(jason);
			game->GetService<CCamera>()->SetTarget(jason);

			jason->SetEnable(true);
			jason->SetPosition(transform.position);
			jason->SetControllable(true);
			jason->SetDirection(nx);
			jason->SetState(JasonState::JASON_JUMPING);
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

	DWORD now = GetTickCount();
	if (now - lastTimeTakeDamage > untouchalbeTime && untouchable == true)
	{
		untouchable = false;
		tag = ObjectTag::Player;
	}
}

void CSophia::Render()
{
	stateAction->Render();
	leftWheel->Render();
	rightWheel->Render();
	middle->Render();
	cabin->Render();
	gun->Render();
}

void CSophia::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;

	if (dynamic_cast<CBrick*>(other))
	{
		if (onGround == false && collision->ny == 1) onGround = true;
	}
	else if (dynamic_cast<CEnemy*>(other))
	{
		DebugOut(L"Collide with enemy\n");
		if (untouchable == false)
		{
			lastTimeTakeDamage = GetTickCount();
			untouchable = true;

			// TODO: is pushed in the direction of the enemy's movement
		}
	}
}

void CSophia::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
}
