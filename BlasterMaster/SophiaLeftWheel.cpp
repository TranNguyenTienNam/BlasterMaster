#include "SophiaLeftWheel.h"
#include "Animations.h"
#include "SophiaIdleState.h"
#include "SophiaMoveLeftState.h"
#include "SophiaMoveRightState.h"

void CSophiaLeftWheel::InitAnimations()
{
	auto animations = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation(ANIM_LEFT_WHEEL, animations->Get("ani-sophia-left-wheel"));
}

CSophiaLeftWheel::CSophiaLeftWheel(CSophia* sophia)
{
	parent = sophia;
	InitAnimations();

}

CSophiaLeftWheel::~CSophiaLeftWheel()
{
}

void CSophiaLeftWheel::Update(DWORD dt)
{
	nx = parent->GetDirection();
}

void CSophiaLeftWheel::Render()
{
	animations.at(ANIM_LEFT_WHEEL)->Render(transform.position + parent->GetPosition(), -1, layer_index);
}
