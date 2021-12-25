#pragma once
#include "Enemy.h"

enum class Z88State
{
	Awaking,
	Operating,
	Sleeping,
};

class CBossZ88 : public CEnemy
{
protected:
	static const int maxCloneCount = 16;
	static int existingCloneCount;
	static int uncalledCloneCount;			// number of uncalled clone
	static CBossZ88* operatingClone;
	static std::unordered_map<int, CBossZ88*> existingClones;

	static DWORD lastTimeGenerate; //temp

	const Vector2 DEFAULT_SIZE = Vector2(32.0f, 32.0f);

	bool isSleeping;
	Z88State state;

	void InitAnimations();
	void InitColliders();

	void Appear();				// Call only once when player enters into boss room
	void Awaking();
	void Sleeping();
	//void OnDestroy();
	void OnDefeat();

	void WakeAnotherUp();
	void GenerateNewClone();

	void OnlyMoving();
	void MovingAndShooting();
	void OnlyShooting();
public:
	CBossZ88();
	
	void Update(DWORD dt);
	void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

