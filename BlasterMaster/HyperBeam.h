#pragma once
#include "Bullet.h"

#define HYPERBEAM_X "HyperBeam-X"
#define HYPERBEAM_Y "HyperBeam-Y"

class CHyperBeam : public CBullet
{
protected:
	void InitSprites();
public:
	CHyperBeam();
	~CHyperBeam();

	virtual void Update(DWORD dt);
	virtual void Render();
};

