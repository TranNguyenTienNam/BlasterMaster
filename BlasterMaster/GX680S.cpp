#include "GX680S.h"
#include "Animations.h"

void CGX680S::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("GX680S", animations->Get("ani-gx680s"));
}

CGX680S::CGX680S()
{
	InitAnimations();
}

CGX680S::~CGX680S()
{
}

void CGX680S::Update(DWORD dt)
{
}

void CGX680S::Render()
{
	animations.at("GX680S")->Render(transform.position, 1);
}
