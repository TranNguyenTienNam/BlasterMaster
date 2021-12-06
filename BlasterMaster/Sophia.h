#pragma once
#include "GameObject.h"
#include "SophiaState.h"
#include "Playable.h"
#include "SophiaLeftWheel.h"
#include "SophiaRightWheel.h"
#include "SophiaMiddle.h"
#include "SophiaCabin.h"
#include "SophiaGun.h"

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
	const float MOVE_SPEED = 0.15f;
	const float MOVE_ACCELERATION = 0.0002f;
	const float JUMP_SPEED = 0.7f;
	const float GRAVITY = -0.0026f;

	ISophiaState* stateAction;
	ISophiaState* stateDirection;
	//ISophiaState* equipment; // Dont need to code this if Ms.Trinh not require

	DWORD lastTimeToLiftGun;
	DWORD lastTimeToLowerGun;

	// Power, Hover in class Player
	virtual void InitColliders();
public:
	CSophia();

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