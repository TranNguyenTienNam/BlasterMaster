#pragma once
#include "Enemy.h"

enum class Z88State
{
	Awaking,			//	Play open eye animation
	AfterAwaking,		//	Choose one of three action when this animation finished

	OnlyMoving,			//
	OnlyShooting,		//	Action
	MovingAndShooting,	//

	PreSleeping,		//	Play close eye animation 

	Sleeping,			//	Play sleeping animation. When close eye animation finished,
						//	waking other clone up or generating a new clone immediately

	Defeated,
};

class CBossZ88 : public CEnemy
{
protected:
	static const int maxCloneCount = 2;
	static int uncalledCloneCount;			// number of uncalled clone
	static CBossZ88* operatingClone;
	static std::unordered_map<int, CBossZ88*> existingClones;

	const Vector2 DEFAULT_SIZE = Vector2(31.6f, 31.6f);
	const float STICKY_DISTANCE = 3.0f;
	const float MOVING_SPEED = 0.05f;

	int index;
	bool isSleeping;
	Z88State state;
	bool blockLeft, blockRight, blockTop, blockBot;

	// Only shooting state
	const DWORD shootingDelay = 300;
	const int maxShootTimes = 3;
	DWORD lastTimeShooting;
	int shootTimes;
	float angleBullet;
	Vector2 velocityBullet;

	// Defeated state
	const int explosionCount = 25;
	const int maxExplosionAtTheSameTime = 3;
	const int explosionRadius = 40;
	const DWORD explosionDelay = 300;
	int remainingExplosionCount;
	DWORD lastTimeExplosion;

	void InitAnimations();
	void InitColliders();

	void OnAppearing();				// Call only once when player enters into boss room
	void OnAwaking();
	void OnSleeping();
	void OnDestroy();
	void OnDefeat();

	void FindShootingDirection();
	void OnPrepareToSleep();
	void FixPosition();

	void NextClone();
	void WakeAnotherUp();
	void GenerateNewClone();

	void FindingMovingDirection();
	void OperatingBehavior();
	void OnlyMoving();
	void MovingAndShooting();
	void OnlyShooting();
public:
	CBossZ88();

	void SetState(Z88State nextState);
	void TakeDamage(int damage);

	void Update(DWORD dt);
	void Render();

	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};

