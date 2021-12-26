#include "BossZ88.h"
#include "Animations.h"
#include "Brick.h"

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
	tag = ObjectTag::BossZ88;
	SetState(Z88State::Awaking);
	blockLeft = false, blockRight = false;
	blockTop = false, blockBot = false;
	lastTimeShooting = GetTickCount();
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
		shootTimes = 0;
		break;
	case Z88State::MovingAndShooting:
		break;
	case Z88State::PreSleeping:
		OnPrepareToSleep();
		break;
	case Z88State::Sleeping:
		OnSleeping();
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
	colliders.at(0)->SetDynamic(true);
	animations.at("Operating")->SetIsReversed(false);
	animations.at("Operating")->SetIsFinished(false);
}

void CBossZ88::OnSleeping()
{
	// TODO: Instantiating new clone or enable another
	isSleeping = true;
	colliders.at(0)->SetDynamic(false);
	velocity = VectorZero();
	blockLeft = false, blockRight = false;
	blockTop = false, blockBot = false;

	FixPosition();

	if (uncalledCloneCount > 0)
	{
		if (rand() % 2 == 0)
		{
			GenerateNewClone();
		}
		else
		{
			if (uncalledCloneCount < 15) WakeAnotherUp();
		}
	}
	else
	{
		WakeAnotherUp();
	}
}

//void CBossZ88::OnDestroy()
//{
//}

void CBossZ88::OnDefeat()
{
}

void CBossZ88::OnPrepareToSleep()
{
	animations.at("Operating")->SetIsReversed(true);
	animations.at("Operating")->SetIsFinished(false);

	DebugOut(L"Clones %d\n", existingClones.size());
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

void CBossZ88::WakeAnotherUp()
{
	int randomCloneIndex = -1;
	do
	{
		randomCloneIndex = CMath::Random(1, existingClones.size());
	} 
	while (randomCloneIndex == index);

	existingClones.at(randomCloneIndex)->SetState(Z88State::Awaking);

	DebugOut(L"Wake another up\n");
}

void CBossZ88::GenerateNewClone()
{
randomAgain:

	float randomPosX = CMath::Random(0, 5) * 32 + 48;
	float randomPosY = CMath::Random(0, 5) * 32 + 32;
	Vector2 generatePos = Vector2(randomPosX, randomPosY);

	for (auto map_clone : existingClones)
	{
		auto clone = (CBossZ88*)map_clone.second;
		auto clonePos = clone->GetPosition();
		if (generatePos == clonePos)
		{
			goto randomAgain;
		}
	}

	Instantiate<CBossZ88>(generatePos);
	DebugOut(L"New clone\n");
}

void CBossZ88::OnDestroy()
{
	isEnabled = false;
	isDestroyed = true;

	existingClones.erase(index);
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
		if (CMath::Random(1, 100) > 65)
		{
			SetState(Z88State::OnlyMoving);
		}
		else
		{
			SetState(Z88State::MovingAndShooting);
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
	default:
		break;
	}
}

void CBossZ88::Render()
{
	if (isSleeping == false)
	{
		animations.at("Operating")->Render(transform.position, 1, layer_index);
	}
	else
	{
		animations.at("Sleeping")->Render(transform.position, 1, layer_index);
	}
}

void CBossZ88::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CBrick*>(other) || dynamic_cast<CBossZ88*>(other))
	{
		SetState(Z88State::PreSleeping);
	}
}
