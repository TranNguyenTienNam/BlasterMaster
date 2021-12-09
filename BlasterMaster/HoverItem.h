#pragma once
#include "AbstractItem.h"

class CHoverItem : public CAbstractItem
{
private:
	void InitSprites();
public:
	CHoverItem();
	void GetEffect();
	void Render();
};

