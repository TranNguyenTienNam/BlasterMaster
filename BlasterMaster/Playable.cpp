#include "Playable.h"
#include "GameObject.h"

CJason* CPlayable::jason = nullptr;
CSophia* CPlayable::sophia = nullptr;
DWORD CPlayable::switchDelay = 300;
DWORD CPlayable::lastTimeSwitch = 0;

CPlayable::CPlayable()
{
	power = maxPower;
}

void CPlayable::TakeDamage(int damage)
{
	power -= damage;
	if (power <= 0)
	{
		power = 0;
		isEnabled = false;
		isDestroyed = true;
	}
}
