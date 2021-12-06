#pragma once
#include <Windows.h>
#include <dinput.h>

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

class CPlayable
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

	DWORD untouchalbeTime = 300;
	DWORD lastTimeTakeDamage;
public:
	void SetControllable(bool value) { this->controllable = value; }
};