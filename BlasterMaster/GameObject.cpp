#include "GameObject.h"

CGameObject::CGameObject()
{
	transform.position.x = transform.position.y = 0;
	velocity.x = velocity.y = 0;
}

CGameObject::~CGameObject()
{
	
}

void CGameObject::AddAnimation(std::string stateName, LPANIMATION animation)
{
	animations.insert(std::make_pair(stateName, animation));
}