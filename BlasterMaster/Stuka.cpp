#include "Stuka.h"
#include "Animations.h"

void CStuka::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Stuka", animations->Get("ani-stuka"));
}

CStuka::CStuka()
{
	InitAnimations();
}

CStuka::~CStuka()
{
}

void CStuka::Update(DWORD dt)
{
}

void CStuka::Render()
{
	animations.at("Stuka")->Render(transform.position, nx);
}