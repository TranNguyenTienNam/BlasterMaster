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
	SHOOTING_KEY = DIK_Z,
	SWITCH_CHARACTER_KEY = DIK_LSHIFT,
	SOPHIA_UPWARD_KEY = DIK_UP
};

class CPlayable : public CGameObject
{
protected:
	static CJason* jason;
	static CSophia* sophia;
	static DWORD switchDelay;
	static DWORD lastTimeSwitch;

	bool controllable;
	bool untouchable;
	bool onGround;
	int power;			// HP

	DWORD untouchalbeTime = 5000;
	DWORD lastTimeTakeDamage;
public:
	bool IsInvincible() { return this->untouchable; }
	void SetControllable(bool value) { this->controllable = value; }
};