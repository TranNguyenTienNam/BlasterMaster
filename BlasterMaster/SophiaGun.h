#pragma once
#include "Sophia.h"

#define SPR_GUN_00 "Gun-00"
#define SPR_GUN_45 "Gun-45"
#define SPR_GUN_90 "Gun-90"

class CSophiaGun : public CGameObject
{
protected:
	CSophia* parent;
	void InitSprites();
public:
	CSophiaGun(CSophia* sophia);
	~CSophiaGun();
	virtual void Update(DWORD dt);
	virtual void Render();
};