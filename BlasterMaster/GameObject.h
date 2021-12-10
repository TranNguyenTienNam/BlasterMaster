#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d9.h>
#include <d3dx9.h>

#include "Animation.h"
#include "Transform.h"
#include "Quadtree.h"
#include "Collider2D.h"
#include "Utils.h"
#include "ObjectTag.h"

class CQuadtree;
class CCollider2D;
struct CCollisionEvent;

class CGameObject
{
protected:
	bool isEnabled = true;
	bool isDestroyed = false;

	ObjectTag tag;
	int layer_index = 1;

	Transform transform;
	Vector2 velocity;
	Vector2 acceleration;
	std::vector<CCollider2D*>  colliders;
	
	int nx;

	CQuadtree* ownerQuadtree;		// Quadtree
	int inNodesIndex = -1;			//

	std::unordered_map<std::string, LPSPRITE> sprites;
	std::unordered_map<std::string, LPANIMATION> animations;

	D3DCOLOR damagedColor[5] =
	{
		D3DCOLOR_ARGB(255, 255, 255, 255),
		D3DCOLOR_ARGB(255, 148, 247, 207),
		D3DCOLOR_ARGB(128, 124, 124, 124),
		D3DCOLOR_ARGB(255, 247, 164, 143),
		D3DCOLOR_ARGB(128, 124, 124, 124),
	};
public:
	CGameObject();
	virtual ~CGameObject();

	bool IsEnabled() { return this->isEnabled; }
	void SetEnable(bool value) { this->isEnabled = value; }
	bool IsDestroyed() { return this->isDestroyed; }
	void SetDestroyed() { this->isDestroyed = true; }
	ObjectTag GetTag() { return this->tag; }

	int GetDirection() { return this->nx; }
	void SetDirection(int value) { this->nx = value; }
	void SetPosition(Vector2 pos) { this->transform.position = pos; }
	Vector2 GetPosition() { return this->transform.position; }
	void SetVelocity(Vector2 v) { this->velocity = v; }
	Vector2 GetVelocity() { return this->velocity; }
	void SetAcceleration(Vector2 a) { this->acceleration = a; }
	Vector2 GetAcceleration() { return this->acceleration; }

	std::vector<CCollider2D*> GetColliders() { return this->colliders; }
	void SetColliders(std::vector<CCollider2D*> colliders) { this->colliders = colliders; }

	CQuadtree* GetQuadtree() { return this->ownerQuadtree; }
	void SetQuadtree(CQuadtree* quadtree) { this->ownerQuadtree = quadtree; }
	int GetInNodesIndex() { return this->inNodesIndex; }
	void SetInNodesIndex(int index) { this->inNodesIndex = index; }

	void AddSprite(std::string stateName, LPSPRITE sprite);
	void AddAnimation(std::string stateName, LPANIMATION animation);
	std::unordered_map<std::string, LPANIMATION> GetAnimations() { return animations; }

	virtual void PhysicsUpdate(std::vector<CGameObject*>* coObjects);
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	virtual void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	virtual void OnDestroy();
};

template<typename T>
inline T* Instantiate(Vector2 position)
{
	CGameObject* newObject = new T;
	newObject->SetPosition(position);
	CGame::GetInstance()->AddGameObject(newObject);
	return (T*)newObject;
}

#endif