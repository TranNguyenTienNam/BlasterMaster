#include "GX680.h"
#include "Animations.h"

void CGX680::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("GX680", animations->Get("ani-gx680"));
}

CGX680::CGX680()
{
	InitAnimations();
}

CGX680::~CGX680()
{
}

void CGX680::Update(DWORD dt)
{
}

void CGX680::Render()
{
	animations.at("GX680")->Render(transform.position, 1);
}