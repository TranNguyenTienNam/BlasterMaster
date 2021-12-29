#pragma once
#include "Sophia.h"

#define SPR_CABIN_00 "Cabin-00"
#define SPR_CABIN_UPWARD "Cabin-Upward"
#define SPR_CABIN_TURN "Cabin-Turn"
#define SPR_CABIN_OPEN "Cabin-Open"

class CSophiaCabin : public CGameObject
{
protected:
	const DWORD switchingDuration = 300;
	DWORD lastTimeSwitching;
	CSophia* parent;
	bool isSwitching;
	void InitSprites();
public:
	CSophiaCabin(CSophia* sophia);
	~CSophiaCabin();
	
	void Switching();

	virtual void Update(DWORD dt);
	virtual void Render();
};

