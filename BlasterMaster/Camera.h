#pragma once
#include "Service.h"
#include "Transform.h"
#include "GameObject.h"

class CCamera : public CService
{
protected:
	Vector2 position;
	RectF boundary;
	Vector2 bbSize;
	CGameObject* target;
public:
	CCamera();
	~CCamera();
	Vector2 WorldToScreenPoint(Vector2 pos);
	Vector2 GetPosition() { return this->position; }
	void SetPosition(Vector2 pos) { this->position = pos; }
	void SetBoundingBoxSize(Vector2 boxSize) { this->bbSize = boxSize; }
	RectF GetBoundingBox();
	void GetBoundary(RectF boundary) { this->boundary = boundary; }
	CGameObject* GetTarget() { return this->target; }
	void SetTarget(CGameObject* target) { this->target = target; }

	void Update();
};