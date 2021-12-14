#include "Math.h"

const float CMath::PI = 3.14f;

int CMath::Random(int start, int end)
{
	return rand() % (end - start + 1) + start;
}

float CMath::CalcDistance(Vector2 pos1, Vector2 pos2)
{
	return sqrt((pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2)));
}
