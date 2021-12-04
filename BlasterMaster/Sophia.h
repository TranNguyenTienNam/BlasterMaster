#pragma once
#include "GameObject.h"
#include "SophiaState.h"
#include "Playable.h"
#include "SophiaLeftWheel.h"
#include "SophiaRightWheel.h"
#include "SophiaMiddle.h"
#include "SophiaCabin.h"
#include "SophiaGun.h"

#define MOVE_SPEED 1000.15f
#define MOVE_ACCELERATION 0.02f
#define JUMP_SPEED 0.7f
#define GRAVITY -0.0026f

class CPlayer;

class ISophiaState;
class CSophiaLeftWheel;
class CSophiaRightWheel;
class CSophiaMiddle;
class CSophiaCabin;
class CSophiaGun;

class CSophia : public CGameObject, public CPlayable
{
private:
	ISophiaState* stateAction;
	ISophiaState* stateDirection;
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

	ISophiaState* GetActionState() { return this->stateAction; }
	ISophiaState* GetDirectionState() { return this->stateDirection; }

	virtual void Update(DWORD dt);
	virtual void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};