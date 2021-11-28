#pragma once
#include "Sophia.h"

class CSophia;

class ISophiaState
{
protected:
	static CSophia* owner;
public:
	void SetOwner(CSophia* sophia) { owner = sophia; }
	virtual void Shooting();
	virtual void UpdateColliders();
	virtual void Update(DWORD dt);
	virtual void Render();
};
