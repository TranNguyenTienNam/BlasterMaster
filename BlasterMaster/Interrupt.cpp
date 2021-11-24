#include "Interrupt.h"

void CInterrupt::InitAnimations()
{
	auto sprite_manager = CGame::GetInstance()->GetService<CSprites>();
	sprites.insert(std::make_pair("Close", sprite_manager->Get("spr-interrupt-close")));
	sprites.insert(std::make_pair("Open", sprite_manager->Get("spr-interrupt-open")));
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
