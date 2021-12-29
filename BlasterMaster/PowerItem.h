#pragma once
#include "AbstractItem.h"

class CPowerItem : public CAbstractItem
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(16.0f, 16.0f);
	void InitSprites();
public:
	CPowerItem();
	void GetEffect(CPlayable* picker);
	void Render();
};

