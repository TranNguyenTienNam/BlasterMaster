#include "Jason.h"
#include "Animations.h"
#include "InputHandler.h"

void CJason::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Idle", animations->Get("ani-jason-idle"));
	AddAnimation("Walk", animations->Get("ani-jason-walk"));
	AddAnimation("Jump", animations->Get("ani-jason-jump"));
}

CJason::CJason()
{
	InitAnimations();
	SetState(JasonState::IDLE);
	onGround = true;
}

CJason::~CJason()
{
}

void CJason::SetState(JasonState state)
{
	switch (state)
	{
	case IDLE:
		velocity.x = 0.0f;
		if (onGround == true) animation = animations.at("Idle");
		break;
	case WALKING_LEFT:
		velocity.x = -JASON_WALKING_SPEED;
		nx = -1;
		if (onGround == true) animation = animations.at("Walk");
		break;
	case WALKING_RIGHT:
		velocity.x = JASON_WALKING_SPEED;
		nx = 1;
		if (onGround == true) animation = animations.at("Walk");
		break;
	case JASON_JUMPING:
		onGround = false;
		velocity.y = JASON_JUMP_SPEED_Y;
		animation = animations.at("Jump");
		break;
	default:
		break;
	}
}

void CJason::Update(DWORD dt)
{
	if (controllable == false)
	{
		SetState(JasonState::IDLE);
		return;
	}

	transform.position.x += velocity.x * dt;
	transform.position.y += velocity.y * dt;
	if (transform.position.y <= 50)
	{
		onGround = true;
		transform.position.y = 50;
	}

	velocity.y -= JASON_GRAVITY * dt;

	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();
	if (inputHandler->IsKeyDown(DIK_RIGHT))
	{
		SetState(JasonState::WALKING_RIGHT);
	}
	else if (inputHandler->IsKeyDown(DIK_LEFT))
	{
		SetState(JasonState::WALKING_LEFT);
	}
	else
	{
		SetState(JasonState::IDLE);
	}

	if (inputHandler->OnKeyDown(DIK_X) && onGround == true)
	{
		SetState(JasonState::JASON_JUMPING);
	}
}

void CJason::Render()
{
	animation->Render(transform.position, nx);
}
