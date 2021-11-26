#include "Jason.h"
#include "Animations.h"
#include "InputHandler.h"
#include "Brick.h"
#include "Utils.h"
#include "Sophia.h"

void CJason::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Idle", animations->Get("ani-jason-idle"));
	AddAnimation("Walk", animations->Get("ani-jason-walk"));
	AddAnimation("Jump", animations->Get("ani-jason-jump"));
}

CJason::CJason() :CGameObject()
{
	InitAnimations();

	// Init collider
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(Vector2(JASON_WIDTH, JASON_HEIGHT));
	collider->SetDynamic(true);
	colliders.push_back(collider);

	// Player's settings
	onGround = true;
	controllable = false;
	jason = this;
	SetState(JasonState::JASON_IDLE);
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
		/*acceleration.x = 0.0f;*/
		if (onGround == true) animation = animations.at("Idle");
		break;
	case JASON_MOVING_LEFT:
		/*velocity.x = -JASON_WALKING_SPEED;*/
		acceleration.x = -0.0002f;
		nx = -1;
		if (onGround == true && velocity.x != 0) animation = animations.at("Walk");
		break;
	case JASON_MOVING_RIGHT:
		/*velocity.x = JASON_WALKING_SPEED;*/
		acceleration.x = 0.0002f;
		nx = 1;
		if (onGround == true && velocity.x != 0) animation = animations.at("Walk");
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
	velocity.y += -0.0026f * dt;
	velocity.x += acceleration.x * dt;


	// TODO: Limit velocity
	/*if (velocity.x > JASON_WALKING_SPEED) velocity.x = JASON_WALKING_SPEED;
	else if (velocity.x < -JASON_WALKING_SPEED) velocity.x = -JASON_WALKING_SPEED;*/

	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();
	if (controllable == false)
	{
		SetState(JasonState::JASON_IDLE);
		return;
	}

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
}

void CJason::Render()
{
	animation->Render(transform.position, -nx);
}

void CJason::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	if (dynamic_cast<CBrick*>(collision->obj))
	{
		if (onGround == false && collision->ny == 1) onGround = true;
		// TODO: Collise with wall, then hold idle state
	}
}

void CJason::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collisions)
{
}
