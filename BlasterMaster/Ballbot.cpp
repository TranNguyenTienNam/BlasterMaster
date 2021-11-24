#include "Ballbot.h"
#include "Animations.h"

void CBallbot::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Ballbot", animations->Get("ani-ballbot"));
}

CBallbot::CBallbot()
{
	InitAnimations();
}

CBallbot::~CBallbot()
{
}

void CBallbot::Update(DWORD dt)
{
}

void CBallbot::Render()
{
	animations.at("Ballbot")->Render(transform.position, nx);
}