#include "SophiaRightWheel.h"
#include "Animations.h"
#include "SophiaIdleState.h"
#include "SophiaMoveLeftState.h"
#include "SophiaMoveRightState.h"

void CSophiaRightWheel::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation(ANIM_RIGHT_WHEEL, animations->Get("ani-sophia-right-wheel"));
}

CSophiaRightWheel::CSophiaRightWheel(CSophia* sophia)
{
	parent = sophia;
	InitAnimations();
}

CSophiaRightWheel::~CSophiaRightWheel()
{
}

void CSophiaRightWheel::Update(DWORD dt)
{
	nx = parent->GetDirection();
}

void CSophiaRightWheel::Render()
{
	animations.at(ANIM_RIGHT_WHEEL)->Render(transform.position + parent->GetPosition(), -1, layer_index);
}