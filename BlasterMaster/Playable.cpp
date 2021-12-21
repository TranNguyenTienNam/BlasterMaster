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

void CPlayable::OnDead()
{

}

void CPlayable::AffectPowerAttribute(int value)
{
	power += value;
	if (power <= 0)
	{
		power = 0;
		OnDead();
	}

	if (power >= maxPower)
	{
		power = maxPower;
	}
}
