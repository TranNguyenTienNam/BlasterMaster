#include "HyperBeam.h"

void CHyperBeam::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetService<CSprites>();
	AddSprite(HYPERBEAM_X, sprites->Get("spr-hyperbeam-axisx"));
	AddSprite(HYPERBEAM_Y, sprites->Get("spr-hyperbeam-axisy"));
}

CHyperBeam::CHyperBeam()
{
	InitSprites();
}

CHyperBeam::~CHyperBeam()
{
}

void CHyperBeam::Update(DWORD dt)
{
	transform.position.x += velocity.x * dt;
	transform.position.y += velocity.y * dt;
}

void CHyperBeam::Render()
{
	if (velocity.x != 0) sprites.at(HYPERBEAM_X)->Draw(transform.position, -nx, 255);
	else if (velocity.y != 0) sprites.at(HYPERBEAM_Y)->Draw(transform.position, 1, 255);
}
