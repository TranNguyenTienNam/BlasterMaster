#pragma once
#include "Service.h"
#include "Transform.h"
#include "GameObject.h"

class CGameObject;

enum class CameraState
{
	FreePlaying,
	Switching_BlockTop,
	Switching_BlockBottom,
	Switching_NoneBlock,
};

class CCamera : public CService
{
protected:
	const DWORD switchingDuration = 2500;

	Vector2 position;
	RectF boundary;
	RectF freezeBoundary = RectF(104, 64, 152, 160);
	Vector2 bbSize;
	CGameObject* target;

	CameraState state;
	bool isLeftToRight;
	Vector2 velocitySwitching;
	float boundary_blocking;
	float boundary_stopping;
public:
	CCamera();
	~CCamera();
	Vector2 WorldToScreenPoint(Vector2 pos);

	Vector2 GetPosition() { return this->position; }
	void SetPosition(Vector2 pos) { this->position = pos; }
	RectF GetBoundingBox();
	Vector2 GetBoundingBoxSize() { return this->bbSize; }
	void SetBoundingBoxSize(Vector2 boxSize) { this->bbSize = boxSize; }
	void GetBoundary(RectF boundary) { this->boundary = boundary; }
	CGameObject* GetTarget() { return this->target; }
	void SetTarget(CGameObject* target) { this->target = target; }

	void PreSwitchingUpdate(Vector2 destination, Vector2 translation);
	void Update();
	void UpdateFreePlaying();
	void UpdateSwitching();
};