#pragma once
#include <stdlib.h>
#include "Transform.h"

class CMath
{
public:
	static const float PI;

	static int Random(int start, int end);	// Get random integer number between start to end
	static float CalcDistance(Vector2 pos1, Vector2 pos2);
};

