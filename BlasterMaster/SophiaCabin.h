#pragma once
#include "Sophia.h"

#define SPR_CABIN_00 "Cabin-00"
#define SPR_CABIN_UPWARD "Cabin-Upward"

class CSophiaCabin : public CGameObject
{
protected:
	CSophia* parent;
	void InitSprites();
public:
	CSophiaCabin(CSophia* sophia);
	~CSophiaCabin();
	
	virtual void Update(DWORD dt);
	virtual void Render();
};

