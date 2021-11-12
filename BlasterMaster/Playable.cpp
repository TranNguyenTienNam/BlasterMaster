#include "Playable.h"
#include "Animations.h"
#include "Utils.h"
#include "Transform.h"

void CPlayable::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Walk", animations->Get("ani-jason"));
	AddAnimation("Clockwise", animations->Get("ani-drap"));
}

CPlayable::CPlayable() :CGameObject()
{
	InitAnimations();
}

CPlayable::~CPlayable()
{
}

void CPlayable::Update(DWORD dt)
{
	transform.position.x += velocity.x * dt;
	transform.position.y += velocity.y * dt;

	/*auto game = CGame::GetInstance();
	if (transform.position.x <= 0)
	{
		transform.position.x = 0;
	}
	if (transform.position.x >= game->GetScreenWidth() - 16)
	{
		transform.position.x = game->GetScreenWidth() - 16;
	}
	if (transform.position.y < 0)
	{
		transform.position.y = 0;
	}
	if (transform.position.y >= game->GetScreenHeight() - 16)
	{
		transform.position.y = game->GetScreenHeight() - 16;
	}*/
}

void CPlayable::Render()
{
	Vector2 pos = transform.position;
	animations.at("Walk")->Render(pos, nx);
}

void CPlayable::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PLAYER_STATE_WALKING_RIGHT:
		velocity = Vector2(PLAYER_WALKING_SPEED, 0);
		nx = 1;
		break;
	case PLAYER_STATE_WALKING_LEFT:
		velocity = Vector2(-PLAYER_WALKING_SPEED, 0);
		nx = -1;
		break;
	case PLAYER_STATE_WALKING_TOP:
		velocity = Vector2(0, PLAYER_WALKING_SPEED);
		break;
	case PLAYER_STATE_WALKING_DOWN:
		velocity = Vector2(0, -PLAYER_WALKING_SPEED);
		break;
	case PLAYER_STATE_IDLE:
		velocity = Vector2(0, 0);
		break;
	}
}