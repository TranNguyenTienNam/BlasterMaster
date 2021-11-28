#pragma once
#include "Sophia.h"

#define SPR_MIDDLE "Middle"

class CSophiaMiddle : public CGameObject
{
protected:
	CSophia* parent;
	void InitSprites();
public:
	CSophiaMiddle(CSophia* sophia);
	~CSophiaMiddle();
	virtual void Update(DWORD dt);
	virtual void Render();
};