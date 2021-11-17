#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d9.h>
#include <d3dx9.h>

#include "Animation.h"
#include "Transform.h"
#include "Quadtree.h"

class CQuadtree;

class CGameObject
{
protected:
	Transform transform;
	Vector2 velocity;
	
	int nx;

	int state;

	CQuadtree* ownerQuadtree;		// Quadtree
	int inNodesIndex = -1;			//

	std::unordered_map<std::string, LPANIMATION> animations;

public:
	CGameObject();
	~CGameObject();

	void SetPosition(Vector2 pos) { this->transform.position = pos; }
	Vector2 GetPosition() { return this->transform.position; }
	void SetSpeed(Vector2 v) { this->velocity = v; }
	Vector2 GetSpeed() { return this->velocity; }
	int GetState() { return this->state; }
	void SetState(int state) { this->state = state; }

	CQuadtree* GetQuadtree() { return this->ownerQuadtree; }
	void SetQuadtree(CQuadtree* quadtree) { this->ownerQuadtree = quadtree; }
	int GetInNodesIndex() { return this->inNodesIndex; }
	void SetInNodesIndex(int index) { this->inNodesIndex = index; }

	void AddAnimation(std::string stateName, LPANIMATION animation);
	std::unordered_map<std::string, LPANIMATION> GetAnimations() { return animations; }

	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
};

#endif