#pragma once
#include "GameObject.h"

class CInterrupt : public CGameObject
{
private:
	std::unordered_map<std::string, LPSPRITE> sprites;

	void InitAnimations();
public:
	CInterrupt();
	~CInterrupt();

	void Update(DWORD dt);
	void Render();
};