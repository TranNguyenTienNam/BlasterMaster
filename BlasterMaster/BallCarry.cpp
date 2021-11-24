#include "BallCarry.h"
#include "Animations.h"

void CBallCarry::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("BallCarry", animations->Get("ani-ballcarry"));
}

CBallCarry::CBallCarry()
{
	InitAnimations();
}

CBallCarry::~CBallCarry()
{
}

void CBallCarry::Update(DWORD dt)
{
}

void CBallCarry::Render()
{
	animations.at("BallCarry")->Render(transform.position, nx);
}
