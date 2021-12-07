#include "Jason.h"
#include "Animations.h"
#include "InputHandler.h"
#include "Brick.h"
#include "Utils.h"
#include "Sophia.h"
#include "Camera.h"
#include "Neoworm.h"

void CJason::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Idle", animations->Get("ani-jason-idle"));
	AddAnimation("Walk", animations->Get("ani-jason-walk"));
	AddAnimation("Jump", animations->Get("ani-jason-jump"));
}

void CJason::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(IDLE_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CJason::CJason()
{
	InitAnimations();
	InitColliders();

	// Player's settings
	isEnabled = false;
	onGround = false;
	controllable = false;
	jason = this;
	SetState(JasonState::JASON_IDLE);
}

void CJason::SetState(JasonState state)
{
	switch (state)
	{
	case JASON_IDLE:
		velocity.x = 0.0f;
		/*acceleration.x = 0.0f;*/
		if (onGround == true) animation = animations.at("Idle");
		break;
	case JASON_MOVING_LEFT:
		acceleration.x = -MOVE_ACCELERATION;
		nx = -1;
		if (onGround == true && velocity.x != 0) animation = animations.at("Walk");
		break;
	case JASON_MOVING_RIGHT:
		acceleration.x = MOVE_ACCELERATION;
		nx = 1;
		if (onGround == true && velocity.x != 0) animation = animations.at("Walk");
		break;
	case JASON_JUMPING:
		onGround = false;
		velocity.y = JUMP_SPEED;
		animation = animations.at("Jump");
		break;
	default:
		break;
	}
}

void CJason::Update(DWORD dt)
{
	velocity.y += GRAVITY * dt;
	velocity.x += acceleration.x * dt;

	// TODO: Limit velocity
	/*if (velocity.x > JASON_WALKING_SPEED) velocity.x = JASON_WALKING_SPEED;
	else if (velocity.x < -JASON_WALKING_SPEED) velocity.x = -JASON_WALKING_SPEED;*/

	if (controllable == false)
	{
		return;
	}

	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();

	if (inputHandler->IsKeyDown(PlayerKeySet::MOVE_RIGHT_KEY))
	{
		SetState(JasonState::JASON_MOVING_RIGHT);
	}
	else if (inputHandler->IsKeyDown(PlayerKeySet::MOVE_LEFT_KEY))
	{
		SetState(JasonState::JASON_MOVING_LEFT);
	}
	else
	{
		SetState(JasonState::JASON_IDLE);
	}

	if (inputHandler->OnKeyDown(PlayerKeySet::JUMPING_KEY) && onGround == true)
	{
		SetState(JasonState::JASON_JUMPING);
	}

	if (inputHandler->OnKeyDown(PlayerKeySet::SWITCH_CHARACTER_KEY) &&
		GetTickCount() - lastTimeSwitch > switchDelay &&
		sophia->GetColliders().at(0)->GetBoundingBox().Contain(colliders.at(0)->GetBoundingBox()))
	{
		lastTimeSwitch = GetTickCount();
		controllable = false;

		transform.position = sophia->GetPosition();
		velocity.x = 0;
		acceleration.x = 0;
		nx = sophia->GetDirection();
		SetState(JasonState::JASON_JUMPING);
	}

	if (inputHandler->OnKeyDown(PlayerKeySet::SHOOTING_KEY))
	{
		Instantiate<CNeoworm>(transform.position); // TODO: Replace neoworm by jason's bullet
	}

	/*DWORD now = GetTickCount();
	if (now - lastTimeTakeDamage > untouchalbeTime && untouchable == true)
	{
		untouchable = false;
	}*/
}

void CJason::Render()
{
	animation->Render(transform.position, -nx, D3DCOLOR_ARGB(255, 124, 255, 124));
}

void CJason::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;

	if (dynamic_cast<CBrick*>(other))
	{
		if (onGround == false && collision->ny == 1) onGround = true;
		// TODO: Collise with wall, then hold idle state
	}
	
	else if (dynamic_cast<CSophia*>(other))
	{
		if (controllable == false && velocity.y < 0)
		{
			isEnabled = false;

			auto game = CGame::GetInstance();
			game->SetPlayer(sophia);
			game->GetService<CCamera>()->SetTarget(sophia);

			sophia->SetControllable(true);
			for (auto co : sophia->GetColliders())
			{
				co->SetTrigger(false);
				co->SetDynamic(true);
			}
		}
	}
	/*else if (dynamic_cast<CEnemy*>(other))
	{
		DebugOut(L"Collide with enemy\n");
		lastTimeTakeDamage = GetTickCount();
		selfCollider->SetTrigger(true);
		if (untouchable == false) untouchable = true;
	}*/
}

void CJason::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
}
