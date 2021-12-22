#include "Jason.h"
#include "Animations.h"
#include "InputHandler.h"
#include "Brick.h"
#include "Utils.h"
#include "Sophia.h"
#include "Camera.h"
#include "PlayScene.h"
#include "Scenes.h"
#include "JasonBullet.h"
#include "Enemy.h"

void CJason::InitAnimations()
{
	auto animation_manager = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Idle", animation_manager->Get("ani-jason-idle"));
	AddAnimation("Walk", animation_manager->Get("ani-jason-walk"));
	AddAnimation("Jump", animation_manager->Get("ani-jason-jump"));
	AddAnimation("Dead", animation_manager->Get("ani-jason-die"));
	animations.at("Dead")->SetIsLooped(false);
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
	tag = ObjectTag::Jason;
	isEnabled = false;
	onGround = false;
	controllable = false;
	jason = this;
	SetState(JasonState::IDLE);
}

void CJason::SetState(JasonState state)
{
	switch (state)
	{
	case JasonState::IDLE:
		velocity.x = 0.0f;
		/*acceleration.x = 0.0f;*/
		if (onGround == true) animation = animations.at("Idle");
		break;
	case JasonState::MOVING_LEFT:
		acceleration.x = -WALK_ACCELERATION;
		nx = -1;
		if (onGround == true && velocity.x != 0) animation = animations.at("Walk");
		break;
	case JasonState::MOVING_RIGHT:
		acceleration.x = WALK_ACCELERATION;
		nx = 1;
		if (onGround == true && velocity.x != 0) animation = animations.at("Walk");
		break;
	case JasonState::JUMPING:
		onGround = false;
		velocity.y = JUMP_SPEED;
		animation = animations.at("Jump");
		break;
	case JasonState::DEAD:
		nx = -1;
		velocity = VectorZero();
		acceleration = VectorZero();
		controllable = false;
		animation = animations.at("Dead");
		break;
	default:
		break;
	}
}

void CJason::Update(DWORD dt)
{
	DebugOut(L"jason pos %f %f\n", transform.position.x, transform.position.y);

	velocity.y += GRAVITY * dt;
	velocity.x += acceleration.x * dt;

	// TODO: Limit velocity
	/*if (velocity.x > WALKING_SPEED) velocity.x = WALKING_SPEED;
	else if (velocity.x < -WALKING_SPEED) velocity.x = -WALKING_SPEED;*/

	if (controllable == false) return;

	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();

	if (inputHandler->IsKeyDown(PlayerKeySet::MOVE_RIGHT_KEY))
	{
		SetState(JasonState::MOVING_RIGHT);
	}
	else if (inputHandler->IsKeyDown(PlayerKeySet::MOVE_LEFT_KEY))
	{
		SetState(JasonState::MOVING_LEFT);
	}
	else
	{
		SetState(JasonState::IDLE);
	}

	if (inputHandler->OnKeyDown(PlayerKeySet::JUMPING_KEY) && onGround == true)
	{
		SetState(JasonState::JUMPING);
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
		SetState(JasonState::JUMPING);
	}

	if (inputHandler->OnKeyDown(PlayerKeySet::SHOOTING_KEY))
	{
		auto bullet = Instantiate<CJasonBullet>(transform.position);
		bullet->SetVelocity(Vector2(nx * bullet->GetSpeed(), 0.0f));
	}

	DWORD now = GetTickCount();
	if (now - lastTimeTakeDamage > untouchalbeTime && untouchable == true)
	{
		untouchable = false;
	}
}

void CJason::Render()
{
	if (controllable == false)
	{
		if (animation->IsFinished() == true)
		{
			isEnabled = false;
			isDestroyed = true;
		}
	}

	animation->Render(transform.position, -nx, layer_index + 1);
}

void CJason::OnDead()
{
	DebugOut(L"[JASON] On Dead\n");
	SetState(JasonState::DEAD);
}

void CJason::OnOverlapped(CCollider2D* selfCollider, CGameObject* object)
{
	if (dynamic_cast<CEnemy*>(object))
	{
		// TODO: is pushed in the direction of the enemy's movement
		AffectPowerAttribute(((CEnemy*)object)->GetDamageOnCollision());
	}
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
			((CPlayScene*)game->GetService<CScenes>()->GetCurrentScene())->SetPlayer(sophia);

			sophia->SetControllable(true);
			for (auto co : sophia->GetColliders())
			{
				co->SetTrigger(false);
				co->SetDynamic(true);
			}
		}
	}
	else if (dynamic_cast<CEnemy*>(other))
	{
		// TODO: is pushed in the direction of the enemy's movement
		AffectPowerAttribute(((CEnemy*)other)->GetDamageOnCollision());
	}
}

void CJason::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
}
