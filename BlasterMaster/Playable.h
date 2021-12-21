#pragma once
#include <dinput.h>
#include "GameObject.h"

class CJason;
class CSophia;

enum PlayerKeySet
{
	MOVE_LEFT_KEY = DIK_LEFT,
	MOVE_RIGHT_KEY = DIK_RIGHT,
	JUMPING_KEY = DIK_X,
	SHOOTING_KEY = DIK_C,
	SWITCH_CHARACTER_KEY = DIK_LSHIFT,
	SOPHIA_UPWARD_KEY = DIK_UP
};

// I don't know why when I hold left and up key as the same time, I can't press Z key

class CPlayable : public CGameObject
{
protected:
	static CJason* jason;
	static CSophia* sophia;
	static DWORD switchDelay;
	static DWORD lastTimeSwitch;

	const int maxPower = 1;

	bool controllable;
	bool untouchable;
	bool onGround;

	int power;			// HP

	DWORD untouchalbeTime = 1000;
	DWORD lastTimeTakeDamage;
public:
	CPlayable();
	bool IsInvincible() { return this->untouchable; }
	void SetControllable(bool value) { this->controllable = value; }
	virtual void OnDead();

	int GetPower() { return this->power; }
	void AffectPowerAttribute(int value);
};