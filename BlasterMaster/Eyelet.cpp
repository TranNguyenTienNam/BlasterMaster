#include "Eyelet.h"
#include "Animations.h"

void CEyelet::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Eyelet", animations->Get("ani-eyelet"));
}

CEyelet::CEyelet()
{
	InitAnimations();
}

CEyelet::~CEyelet()
{
}

void CEyelet::Update(DWORD dt)
{
}

void CEyelet::Render()
{
	animations.at("Eyelet")->Render(transform.position, nx);
}