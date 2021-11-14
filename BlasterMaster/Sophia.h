#pragma once
#include "GameObject.h"
#include "SophiaState.h"

class ISophiaState;

enum SophiaKeySet
{
	MOVE_LEFT = DIK_LEFTARROW,
	MOVE_RIGHT = DIK_RIGHTARROW,
	UPWARD = DIK_UPARROW,
	JUMPING = DIK_X,
	SHOOTING = DIK_Z,
	OPEN_CABIN = DIK_LSHIFT
};

class CSophia : public CGameObject
{
private:
	ISophiaState* stateWheel;
	ISophiaState* stateDirection;
	ISophiaState* statePhysical;
	//ISophiaState* equipment; // Dont need to code this if Ms.Trinh not require

	// Power, Hover in class Player
public:
	CSophia();
	~CSophia();
	void InitAnimation(); // If CSophia inherits from CPlayer, this is virtual function
	virtual void Update(DWORD dt);
	virtual void Render();
};