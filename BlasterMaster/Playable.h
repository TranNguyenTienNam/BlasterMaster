#pragma once
#include "GameObject.h"

class CPlayable
{
public:
	bool controllable;
	void SetControllable(bool value) { controllable = value; }
};