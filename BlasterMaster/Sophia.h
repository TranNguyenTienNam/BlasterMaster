#pragma once
#include "GameObject.h"
#include "SophiaState.h"
#include "Playable.h"
#include "SophiaLeftWheel.h"
#include "SophiaRightWheel.h"
#include "SophiaMiddle.h"
#include "SophiaCabin.h"
#include "SophiaGun.h"

class ISophiaState;
class CPlayer;
class CSophiaLeftWheel;
class CSophiaRightWheel;
class CSophiaMiddle;
class CSophiaCabin;
class CSophiaGun;

class CSophia : public CGameObject, public CPlayable
{
private:
	ISophiaState* stateWheel;
	ISophiaState* stateDirection;
	ISophiaState* statePhysical;
	//ISophiaState* equipment; // Dont need to code this if Ms.Trinh not require

	DWORD lastTimeToLiftGun;
	DWORD lastTimeToLowerGun;

	// Power, Hover in class Player
public:
	CSophia();
	~CSophia();

	CSophiaLeftWheel* leftWheel;
	CSophiaRightWheel* rightWheel;
	CSophiaMiddle* middle;
	CSophiaCabin* cabin;
	CSophiaGun* gun;

	ISophiaState* GetWheelState() { return this->stateWheel; }
	ISophiaState* GetDirectionState() { return this->stateDirection; }

	virtual void Update(DWORD dt);
	virtual void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};