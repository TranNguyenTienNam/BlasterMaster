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

	// Init collider
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(Vector2(JASON_WIDTH, JASON_HEIGHT));
	collider->SetDynamic(true);
	colliders.push_back(collider);

	SetState(JasonState::JASON_IDLE);
	onGround = true;
}

CJason::~CJason()
{
}

void CJason::SetState(JasonState state)
{
	switch (state)
	{
	case JASON_IDLE:
		velocity.x = 0.0f;
		if (onGround == true) animation = animations.at("Idle");
		break;
	case JASON_WALKING_LEFT:
		velocity.x = -JASON_WALKING_SPEED;
		nx = -1;
		if (onGround == true) animation = animations.at("Walk");
		break;
	case JASON_WALKING_RIGHT:
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
	if (transform.position.y <= 50)
	{
		onGround = true;
		transform.position.y = 50;
	}

	velocity.y -= JASON_GRAVITY * dt;

	if (controllable == false)
	{
		SetState(JasonState::JASON_IDLE);
		return;
	}

	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();
	if (inputHandler->IsKeyDown(DIK_RIGHT))
	{
		SetState(JasonState::JASON_WALKING_RIGHT);
	}
	else if (inputHandler->IsKeyDown(DIK_LEFT))
	{
		SetState(JasonState::JASON_WALKING_LEFT);
	}
	else
	{
		SetState(JasonState::JASON_IDLE);
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
