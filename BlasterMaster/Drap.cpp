#include "Drap.h"
#include "Animations.h"
#include "Utils.h"
#include "Transform.h"

void CDrap::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Clockwise", animations->Get("ani-drap"));
}

CDrap::CDrap() :CGameObject()
{
	InitAnimations();

	// Init collider
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(Vector2(DRAP_WIDTH, DRAP_HEIGHT));
	colliders.push_back(collider);
}

CDrap::~CDrap()
{
}

void CDrap::Update(DWORD dt)
{
	/*auto game = CGame::GetInstance();
	if (transform.position.x <= 0)
	{
		transform.position.x = 0;
		SetState(DRAP_STATE_WALKING_TOP);
	}
	else if (transform.position.x >= game->GetScreenWidth() - 16) 
	{
		transform.position.x = game->GetScreenWidth() - 20;
		SetState(DRAP_STATE_WALKING_DOWN);
	}
	
	if (transform.position.y < 0)
	{
		transform.position.y = 0;
		SetState(DRAP_STATE_WALKING_RIGHT);
	}
	else if (transform.position.y >= game->GetScreenHeight() - 16) 
	{
		transform.position.y = game->GetScreenHeight() - 20;
		SetState(DRAP_STATE_WALKING_LEFT);
	}*/
}

void CDrap::Render()
{
	Vector2 pos = transform.position;
	animations.at("Clockwise")->Render(pos, nx);
}

void CDrap::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case DRAP_STATE_WALKING_RIGHT:
		velocity = Vector2(DRAP_WALKING_SPEED, 0);
		break;
	case DRAP_STATE_WALKING_LEFT:
		velocity = Vector2(-DRAP_WALKING_SPEED, 0);
		break;
	case DRAP_STATE_WALKING_TOP:
		velocity = Vector2(0, -DRAP_WALKING_SPEED);
		break;
	case DRAP_STATE_WALKING_DOWN:
		velocity = Vector2(0, DRAP_WALKING_SPEED);
		break;
		break;
	}
}