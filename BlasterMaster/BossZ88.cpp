#include "BossZ88.h"
#include "Animations.h"
#include "Brick.h"
#include "Firework.h"
#include "Z88Bullet.h"
#include "SpinBullet.h"

int CBossZ88::uncalledCloneCount = maxCloneCount;
CBossZ88* CBossZ88::operatingClone = nullptr;
std::unordered_map<int, CBossZ88*> CBossZ88::existingClones = {};

void CBossZ88::InitAnimations()
{
	auto animation_manager = CGame::GetInstance()->GetService<CAnimations>();
	AddAnimation("Operating", animation_manager->Clone("ani-z88"));
	AddAnimation("Sleeping", animation_manager->Clone("ani-red-z88"));

	animations.at("Operating")->SetIsLooped(false);
}

void CBossZ88::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(DEFAULT_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

CBossZ88::CBossZ88()
{
	InitAnimations();
	InitColliders();

	// Enemy setting
	scrollingMap = false;

	// Static setting
	uncalledCloneCount--;
	index = maxCloneCount - uncalledCloneCount;
	existingClones.emplace(std::make_pair(index, this));

	// Self setting
	SetState(Z88State::Awaking);
}

void CBossZ88::TakeDamage(int damage)
{
	if (isSleeping == false)
	{
		hp -= damage;
		if (hp <= 0)
		{
			hp = 0;
			OnDestroy();
		}
	}
}

void CBossZ88::SetState(Z88State nextState)
{
	DebugOut(L"[CLONE %d] Next state %d\n", index, nextState);
	state = nextState;
	switch (state)
	{
	case Z88State::Awaking:
		OnAwaking();
		break;
	case Z88State::AfterAwaking:
		FindingMovingDirection();
		break;
	case Z88State::OnlyMoving:
		break;
	case Z88State::OnlyShooting:
		FindShootingDirection();
		break;
	case Z88State::MovingAndShooting:
		ShootingSpinBullet();
		break;
	case Z88State::PreSleeping:
		OnPrepareToSleep();
		break;
	case Z88State::Sleeping:
		OnSleeping();
		break;
	case Z88State::Defeated:
		lastTimeExplosion = GetTickCount();
		remainingExplosionCount = explosionCount;
		tag = ObjectTag::Unknown;
		break;
	default:
		break;
	}
}

void CBossZ88::OnAppearing()
{
}

void CBossZ88::OnAwaking()
{
	isSleeping = false;
	tag = ObjectTag::BossZ88;
	blockLeft = false, blockRight = false;
	blockTop = false, blockBot = false;

	colliders.at(0)->SetDynamic(true);
	animations.at("Operating")->SetIsReversed(false);
	animations.at("Operating")->SetIsFinished(false);
}

void CBossZ88::OnSleeping()
{
	isSleeping = true;
	tag = ObjectTag::Platform;
	colliders.at(0)->SetDynamic(false);
	velocity = VectorZero();

	FixPosition();
	NextClone();
}

void CBossZ88::OnDestroy()
{
	existingClones.erase(index);

	if (existingClones.size() == 0 && uncalledCloneCount == 0)
	{
		SetState(Z88State::Defeated);
	}
	else
	{
		isEnabled = false;
		isDestroyed = true;
		NextClone();
	}
}

void CBossZ88::OnDefeat()
{
	if (remainingExplosionCount < 0)
	{
		DebugOut(L"BOSS WAS DEFEATED!!!\n");
		isEnabled = false;
		isDestroyed = true;
		return;
	}

	DWORD now = GetTickCount();
	if (now - lastTimeExplosion > explosionDelay)
	{
		lastTimeExplosion = now;

		int explosionCountAtTheSameTime = CMath::Random(1, maxExplosionAtTheSameTime);
		remainingExplosionCount -= explosionCountAtTheSameTime;

		for (int i = 0; i < explosionCountAtTheSameTime; i++)
		{
			int randomNX = (rand() % 2 == 0) ? 1 : -1;
			int randomNY = (rand() % 2 == 0) ? 1 : -1;
			int randomPosX = CMath::Random(5, explosionRadius) * randomNX;
			int randomPosY = CMath::Random(5, explosionRadius) * randomNY;
			Instantiate<CFirework>(transform.position + Vector2(randomPosX, randomPosY));
		}
	}
}

void CBossZ88::OnPrepareToSleep()
{
	animations.at("Operating")->SetIsReversed(true);
	animations.at("Operating")->SetIsFinished(false);
}

void CBossZ88::FixPosition()
{
	int residualX = ((int)transform.position.x - 48) % 32;
	if (residualX < STICKY_DISTANCE)
	{
		transform.position.x = (((int)transform.position.x - 48) / 32) * 32 + 48;
	}
	else if (residualX > 32 - STICKY_DISTANCE)
	{
		transform.position.x = (((int)transform.position.x - 48) / 32 + 1) * 32 + 48;
	}

	int residualY = ((int)transform.position.y - 32) % 32;
	if (residualY < STICKY_DISTANCE)
	{
		transform.position.y = (((int)transform.position.y - 32) / 32) * 32 + 32;
	}
	else if (residualY > 32 - STICKY_DISTANCE)
	{
		transform.position.y = (((int)transform.position.y - 32) / 32 + 1) * 32 + 32;
	}
}

void CBossZ88::NextClone()
{
	DebugOut(L"[CLONE %d] clones %d, uncalls %d\n", index, existingClones.size(), uncalledCloneCount);
	if (uncalledCloneCount > 0)
	{
		if (existingClones.size() > 1)
		{
			if (rand() % 2 == 0)
			{
				GenerateNewClone();
			}
			else
			{
				WakeAnotherUp();
			}
		}
		else
		{
			GenerateNewClone();
		}
	}
	else 
	{
		if (existingClones.size() > 0)
		{
			WakeAnotherUp();
		}
	}
}

void CBossZ88::WakeAnotherUp()
{
	DebugOut(L"Wake another up\n");
	int chosenIndex = -1;
	do
	{
		int randomIndex = CMath::Random(0, existingClones.size() - 1);
		int i = 0;
		for (auto map_clone : existingClones)
		{
			if (existingClones.size() > 1 && (CBossZ88*)map_clone.second == this) continue;
			if (i == randomIndex)
			{
				chosenIndex = map_clone.first;
				break;
			}

			i++;
		}
	} while (chosenIndex == -1);
	existingClones.at(chosenIndex)->SetState(Z88State::Awaking);
}

void CBossZ88::GenerateNewClone()
{
	if (target == nullptr) return;

randomAgain:

	float randomPosX = CMath::Random(0, 5) * 32 + 48;
	float randomPosY = CMath::Random(0, 5) * 32 + 32;
	Vector2 generatePos = Vector2(randomPosX, randomPosY);

	for (auto map_clone : existingClones)
	{
		auto clone = (CBossZ88*)map_clone.second;
		auto clonePos = clone->GetPosition();

		float distance = CMath::CalcDistance(generatePos, target->GetPosition());
		if (generatePos == clonePos || distance < 32)
		{
			goto randomAgain;
		}
	}

	Instantiate<CBossZ88>(generatePos);
	DebugOut(L"New clone\n");
}

void CBossZ88::FindingMovingDirection()
{
	for (auto map_clone : existingClones)
	{
		auto clone = (CBossZ88*)map_clone.second;
		if (clone == this) continue;

		auto clonePos = clone->GetPosition();
		if (transform.position.x == clonePos.x)
		{
			if (clonePos.y - transform.position.y <= 64)
			{
				blockTop = true;
			}

			if (transform.position.y - clonePos.y <= 64)
			{
				blockBot = true;
			}
		}

		if (transform.position.y == clonePos.y)
		{
			if (clonePos.x - transform.position.x <= 64)
			{
				blockRight = true;
			}

			if (transform.position.x - clonePos.x <= 64)
			{
				blockLeft = true;
			}
		}
	}

	if (transform.position.x == 48)
	{
		blockLeft = true;
	}

	if (transform.position.x == 208)
	{
		blockRight = true;
	}

	if (transform.position.y == 32)
	{
		blockBot = true;
	}

	if (transform.position.y == 192)
	{
		blockTop = true;
	}

	if (blockTop == true && blockBot == true &&
		blockLeft == true && blockRight == true)
	{
		SetState(Z88State::OnlyShooting);
	}
	else
	{
		// TODO: Increase rate to choose moving and shooting action when boss is in danger
		int actionRate = CMath::Random(1, 100);
		if (actionRate <= 50)
		{
			SetState(Z88State::OnlyMoving);
		}
		else if (actionRate <= 80)
		{
			SetState(Z88State::MovingAndShooting);
		}
		else
		{
			SetState(Z88State::OnlyShooting);
			return;
		}

		if ((blockTop == false || blockBot == false) &&
			(blockLeft == false || blockRight == false))
		{
			if (rand() % 2 == 0)
			{
				if (blockTop == false) velocity = Vector2(0.0f, MOVING_SPEED);
				else if (blockBot == false) velocity = Vector2(0.0f, -MOVING_SPEED);
			}
			else
			{
				if (blockLeft == false) velocity = Vector2(-MOVING_SPEED, 0.0f);
				else if (blockRight == false) velocity = Vector2(MOVING_SPEED, 0.0f);
			}
		}
		else
		{
			if (blockTop == false || blockBot == false)
			{
				if (blockTop == false) velocity = Vector2(0.0f, MOVING_SPEED);
				else if (blockBot == false) velocity = Vector2(0.0f, -MOVING_SPEED);
			}
			else if (blockLeft == false || blockRight == false)
			{
				if (blockLeft == false) velocity = Vector2(-MOVING_SPEED, 0.0f);
				else if (blockRight == false) velocity = Vector2(MOVING_SPEED, 0.0f);
			}
		}

		DebugOut(L"[CLONE %d] velocity %f %f\n", index, velocity.x, velocity.y);
	}
}

void CBossZ88::OperatingBehavior()
{
}

void CBossZ88::OnlyMoving()
{
}

void CBossZ88::MovingAndShooting()
{
}

void CBossZ88::OnlyShooting()
{
	if (shootTimes < maxShootTimes)
	{
		DWORD now = GetTickCount();
		if (now - lastTimeShooting > shootingDelay)
		{
			DebugOut(L"[CLONE %d] Shoot time %d\n", index, shootTimes);
			shootTimes++;
			lastTimeShooting = now;

			auto bullet1 = Instantiate<CZ88Bullet>(transform.position);
			bullet1->SetRotationZ(angleBullet);
			bullet1->SetVelocity(velocityBullet);

			auto bullet2 = Instantiate<CZ88Bullet>(transform.position);
			bullet2->SetRotationZ(angleBullet - 45);
			bullet2->SetVelocity(CMath::Rotating(velocityBullet, 45));

			auto bullet3 = Instantiate<CZ88Bullet>(transform.position);
			bullet3->SetRotationZ(angleBullet + 45);
			bullet3->SetVelocity(CMath::Rotating(velocityBullet, -45));
		}
	}
	else
	{
		SetState(Z88State::PreSleeping);
	}
}

void CBossZ88::Update(DWORD dt)
{
	if (isSleeping == true) return;

	if (animations.at("Operating")->IsFinished())
	{
		if (state == Z88State::Awaking)
		{
			SetState(Z88State::AfterAwaking);
		}
		else if (state == Z88State::PreSleeping)
		{
			SetState(Z88State::Sleeping);
		}
	}

	switch (state)
	{
	case Z88State::Awaking:
		break;
	case Z88State::AfterAwaking:
		break;
	case Z88State::OnlyMoving:
		break;
	case Z88State::OnlyShooting:
		OnlyShooting();
		break;
	case Z88State::MovingAndShooting:
		break;
	case Z88State::PreSleeping:
		break;
	case Z88State::Sleeping:
		break;
	case Z88State::Defeated:
		OnDefeat();
		break;
	default:
		break;
	}

	OnDamagedUpdate();
}

void CBossZ88::Render()
{
	if (state == Z88State::Defeated) return;

	if (isSleeping == false)
	{
		animations.at("Operating")->Render(transform.position, 1, layer_index, 0, damagedColor[colorIndex]);
	}
	else
	{
		animations.at("Sleeping")->Render(transform.position, 1, layer_index);
	}
}

void CBossZ88::FindShootingDirection()
{
	if (target == nullptr) return;

	shootTimes = 0;
	lastTimeShooting = GetTickCount();

	auto targetPos = target->GetPosition();

	if (abs(transform.position.y - targetPos.y) < 90.0f)
	{
		angleBullet = 90;
		if (transform.position.x < targetPos.x)
		{
			velocityBullet = Vector2(0.2f, 0.0f);
		}
		else
		{
			velocityBullet = Vector2(-0.2f, 0.0f);
		}
	}
	else
	{
		angleBullet = 0;
		if (transform.position.y < targetPos.y)
		{
			velocityBullet = Vector2(0.0f, 0.2f);
		}
		else
		{
			velocityBullet = Vector2(0.0f, -0.2f);
		}
	}
}

void CBossZ88::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other) || dynamic_cast<CBossZ88*>(other))
	{
		if (state == Z88State::OnlyMoving ||
			state == Z88State::OnlyShooting ||
			state == Z88State::MovingAndShooting) 
			SetState(Z88State::PreSleeping);
	}
}

void CBossZ88::ShootingSpinBullet()
{
	if (target == nullptr) return;

	auto targetPos = target->GetPosition();
	auto bullet = Instantiate<CSpinBullet>(transform.position);
	auto velocityBullet = CMath::Normalize(targetPos - transform.position) * bullet->GetSpeed();
	bullet->SetVelocity(velocityBullet);
}
