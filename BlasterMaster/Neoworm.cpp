#include "Neoworm.h"
#include "Animations.h"

void CNeoworm::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Move", animations->Get("ani-neoworm"));
}

CNeoworm::CNeoworm()
{
	InitAnimations();
}

CNeoworm::~CNeoworm()
{
}

void CNeoworm::Update(DWORD dt)
{
}

void CNeoworm::Render()
{
	animations.at("Move")->Render(transform.position, nx);
}
