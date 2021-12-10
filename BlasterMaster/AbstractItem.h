#pragma once
#include "GameObject.h"

class CAbstractItem : public CGameObject
{
protected:
	const DWORD aliveDuration = 10000;
	const DWORD preWarningDuration = 7000;
	const int flickeringTimes = 50;

	int alpha = 255;
	std::vector<int> alphaArray = { 100,255,160,255 };
	DWORD timeSpawn;
	DWORD flickeringDuration;
	static int dropRate;
public:
	CAbstractItem();
	static int GetDropRate() { return dropRate; }
	virtual void GetEffect();
	virtual void Update(DWORD dt);
	virtual void Render();
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

