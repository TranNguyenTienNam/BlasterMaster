#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d9.h>
#include <d3dx9.h>

#include "Animation.h"
#include "Transform.h"
#include "Quadtree.h"
#include "Collider2D.h"

class CQuadtree;
class CCollider2D;
struct CCollisionEvent;

class CGameObject
{
protected:
	Transform transform;
	Vector2 velocity;
	Vector2 acceleration;
	std::vector<CCollider2D*>  colliders;
	
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
	void SetVelocity(Vector2 v) { this->velocity = v; }
	Vector2 GetVelocity() { return this->velocity; }
	void SetAcceleration(Vector2 a) { this->acceleration = a; }
	Vector2 GetAcceleration() { return this->acceleration; }
	int GetState() { return this->state; }
	void SetState(int state) { this->state = state; }
	std::vector<CCollider2D*> GetColliders() { return this->colliders; }
	void SetColliders(std::vector<CCollider2D*> colliders) { this->colliders = colliders; }

	CQuadtree* GetQuadtree() { return this->ownerQuadtree; }
	void SetQuadtree(CQuadtree* quadtree) { this->ownerQuadtree = quadtree; }
	int GetInNodesIndex() { return this->inNodesIndex; }
	void SetInNodesIndex(int index) { this->inNodesIndex = index; }

	void AddAnimation(std::string stateName, LPANIMATION animation);
	std::unordered_map<std::string, LPANIMATION> GetAnimations() { return animations; }

	virtual void PhysicsUpdate(std::vector<CGameObject*>* coObjects);
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	virtual void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

#endif