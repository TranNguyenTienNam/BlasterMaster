#pragma once
#include "GX680.h"

class CGX680S : public CGX680
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(18.0f, 16.0f);

	void InitAnimations();
	void InitColliders();
public:
	CGX680S();
	~CGX680S();

	void Render();
};