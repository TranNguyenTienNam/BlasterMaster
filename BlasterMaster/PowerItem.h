#pragma once
#include "AbstractItem.h"

class CPowerItem : public CAbstractItem
{
private:
	void InitSprites();
public:
	CPowerItem();
	void GetEffect();
	void Render();
};
