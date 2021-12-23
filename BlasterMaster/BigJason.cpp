#include "BigJason.h"
#include "Animations.h"
#include "Enemy.h"
#include "BigJasonBullet.h"
#include "Portal.h"
#include "Thorn.h"
#include "GunFlashX.h"
#include "GunFlashY.h"

void CBigJason::InitAnimations()
{
	auto animation_manager = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Idle-Front", animation_manager->Get("ani-bigjason-idle-front"));
	AddAnimation("Idle-Back", animation_manager->Get("ani-bigjason-idle-back"));
	AddAnimation("Idle-Left", animation_manager->Get("ani-bigjason-idle-left"));
	AddAnimation("Walk-Front", animation_manager->Get("ani-bigjason-walk-front"));
	AddAnimation("Walk-Back", animation_manager->Get("ani-bigjason-walk-back"));
	AddAnimation("Walk-Left", animation_manager->Get("ani-bigjason-walk-left"));
	AddAnimation("Dead", animation_manager->Get("ani-bigjason-dead"));
	animations.at("Dead")->SetIsLooped(false);
}

void CBigJason::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(Vector2(0.0f, -10.0f));
	collider->SetBoxSize(IDLE_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CBigJason::CBigJason()
{
	InitAnimations();
	InitColliders();

	// Player's settings
	tag = ObjectTag::BigJason;
	controllable = true;
	SetState(BigJasonState::IDLE_FRONT);
	lastState = currentState;

	layer_index = 2;
}

void CBigJason::SetState(BigJasonState state)
{
	currentState = state;

	switch (currentState)
	{
	case BigJasonState::IDLE_FRONT:
		ny = -1;
		animation = animations.at("Idle-Front");
		break;
	case BigJasonState::IDLE_BACK:
		ny = 1;
		animation = animations.at("Idle-Back");
		break;
	case BigJasonState::IDLE_LEFT:
		nx = -1;
		animation = animations.at("Idle-Left");
		break;
	case BigJasonState::IDLE_RIGHT:
		nx = 1;
		animation = animations.at("Idle-Left");
		break;
	case BigJasonState::WALK_FRONT:
		ny = -1;
		velocity.y = -WALK_SPEED;
		if (lastState != BigJasonState::WALK_LEFT &&
			lastState != BigJasonState::WALK_RIGHT)
			animation = animations.at("Walk-Front");
		break;
	case BigJasonState::WALK_BACK:
		ny = 1;
		velocity.y = WALK_SPEED;
		if (lastState != BigJasonState::WALK_LEFT &&
			lastState != BigJasonState::WALK_RIGHT)
			animation = animations.at("Walk-Back");
		break;
	case BigJasonState::WALK_LEFT:
		nx = -1;
		velocity.x = -WALK_SPEED;
		if (lastState != BigJasonState::WALK_FRONT &&
			lastState != BigJasonState::WALK_BACK)
			animation = animations.at("Walk-Left");
		break;
	case BigJasonState::WALK_RIGHT:
		nx = 1;
		velocity.x = WALK_SPEED;
		if (lastState != BigJasonState::WALK_FRONT &&
			lastState != BigJasonState::WALK_BACK)
			animation = animations.at("Walk-Left");
		break;
	case BigJasonState::DEAD:
		velocity = VectorZero();
		controllable = false;
		animation = animations.at("Dead");
		break;
	default:
		break;
	}

	lastState = currentState;
}

void CBigJason::Shooting()
{
	auto bullet = Instantiate<CBigJasonBullet>(transform.position);

	if (currentState == BigJasonState::IDLE_LEFT || currentState == BigJasonState::WALK_LEFT ||
		currentState == BigJasonState::IDLE_RIGHT || currentState == BigJasonState::WALK_RIGHT)
	{
		bullet->Initialize(Vector2(nx, 0.0f));

		auto effect = Instantiate<CGunFlashX>(transform.position + Vector2(13.0f * nx, 0.0f));
		effect->SetDirection(nx);
	}
	else if (currentState == BigJasonState::IDLE_FRONT || currentState == BigJasonState::WALK_FRONT)
	{
		bullet->Initialize(Vector2(0.0f, ny));

		auto effect = Instantiate<CGunFlashY>(transform.position + Vector2(-5.0f, -13.0f));
		effect->SetRotationZ(180);
	}
	else if (currentState == BigJasonState::IDLE_BACK || currentState == BigJasonState::WALK_BACK)
	{
		bullet->Initialize(Vector2(0.0f, ny));

		auto effect = Instantiate<CGunFlashY>(transform.position + Vector2(7.0f, 7.0f));
		effect->SetRotationZ(0);
	}
}

void CBigJason::Update(DWORD dt)
{
	if (controllable == false) return;

	auto inputHandler = CGame::GetInstance()->GetService<CInputHandler>();
	
	if (inputHandler->IsKeyDown(BigJasonKeySet::MOVE_LEFT))
	{
		SetState(BigJasonState::WALK_LEFT);
	}
	else if (inputHandler->IsKeyDown(BigJasonKeySet::MOVE_RIGHT))
	{
		SetState(BigJasonState::WALK_RIGHT);
	}
	else
	{
		velocity.x = 0;
	}
	
	if (inputHandler->IsKeyDown(BigJasonKeySet::MOVE_TOP))
	{
		SetState(BigJasonState::WALK_BACK);
	}
	else if (inputHandler->IsKeyDown(BigJasonKeySet::MOVE_DOWN))
	{
		SetState(BigJasonState::WALK_FRONT);
	}
	else
	{
		velocity.y = 0;
	}

	if (inputHandler->OnKeyDown(BigJasonKeySet::SHOOTING))
	{
		Shooting();
	}

	if (velocity == VectorZero())
	{
		switch (currentState)
		{
		case BigJasonState::WALK_FRONT:
			SetState(BigJasonState::IDLE_FRONT);
			break;
		case BigJasonState::WALK_BACK:
			SetState(BigJasonState::IDLE_BACK);
			break;
		case BigJasonState::WALK_LEFT:
			SetState(BigJasonState::IDLE_LEFT);
			break;
		case BigJasonState::WALK_RIGHT:
			SetState(BigJasonState::IDLE_RIGHT);
			break;
		default:
			break;
		}	
	}
	else
	{
		if (abs(velocity.x) == abs(velocity.y))
		{
			velocity /= sqrt(2);
		}
	}

	DWORD now = GetTickCount();
	if (now - lastTimeTakeDamage > untouchalbeTime && untouchable == true)
	{
		untouchable = false;
	}
}

void CBigJason::Render()
{
	if (controllable == false)
	{
		int currentFrame = animation->GetCurrentFrame();
		if (currentFrame == 3 || currentFrame == 7)
		{
			nx = 1;
		}
		else
		{
			nx = -1;
		}

		if (animation->IsFinished() == true)
		{
			isEnabled = false;
			isDestroyed = true;
		}
	}

	if (animation == animations.at("Idle-Front") || animation == animations.at("Idle-Back") ||
		animation == animations.at("Walk-Front") || animation == animations.at("Walk-Back"))
		animation->Render(transform.position, 1, layer_index);
	else
		animation->Render(transform.position, -nx, layer_index);
}

void CBigJason::OnDead()
{
	SetState(BigJasonState::DEAD);
}

void CBigJason::OnOverlapped(CCollider2D* selfCollider, CGameObject* object)
{
	if (dynamic_cast<CEnemy*>(object))
	{
		AffectPowerAttribute(((CEnemy*)object)->GetDamageOnCollision());
	}
	else if (dynamic_cast<CThorn*>(object))
	{
		AffectPowerAttribute(((CThorn*)object)->GetDamage());
	}
}

void CBigJason::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CEnemy*>(other))
	{
		DebugOut(L"Collide with enemy, power %d\n", power);
		AffectPowerAttribute(((CEnemy*)other)->GetDamageOnCollision());
	}
	else if (dynamic_cast<CThorn*>(other))
	{
		AffectPowerAttribute(((CThorn*)other)->GetDamage());
	}
}

void CBigJason::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
}
