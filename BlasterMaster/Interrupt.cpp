#include "Interrupt.h"

void CInterrupt::InitAnimations()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite("Close", sprites->Get("spr-interrupt-close"));
	AddSprite("Open", sprites->Get("spr-interrupt-open"));
}

CInterrupt::CInterrupt()
{
	InitAnimations();
}

CInterrupt::~CInterrupt()
{
}

void CInterrupt::Update(DWORD dt)
{
}

void CInterrupt::Render()
{
	sprites.at("Close")->Draw(transform.position, 1, 255);
}
