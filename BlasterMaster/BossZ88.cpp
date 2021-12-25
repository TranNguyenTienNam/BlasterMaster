#include "BossZ88.h"
#include "Animations.h"
#include "Brick.h"

int CBossZ88::uncalledCloneCount = maxCloneCount;
CBossZ88* CBossZ88::operatingClone = nullptr;
std::unordered_map<int, CBossZ88*> CBossZ88::existingClones = {};

DWORD CBossZ88::lastTimeGenerate = 0;

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

void CBossZ88::Appear()
{
}

void CBossZ88::Awaking()
{

}

void CBossZ88::Sleeping()
{
	// TODO: Instantiating new clone or enable another

	isSleeping = true;
}

//void CBossZ88::OnDestroy()
//{
//}

void CBossZ88::OnDefeat()
{
}

void CBossZ88::WakeAnotherUp()
{
	int randomCloneIndex = CMath::Random(1, existingClones.size());
	existingClones.at(randomCloneIndex)->isSleeping = false;
}

void CBossZ88::GenerateNewClone()
{
	if (uncalledCloneCount > 0)
	{
	randomAgain:

		float randomPosX = CMath::Random(0, 5) * 32 + 48;
		float randomPosY = CMath::Random(0, 5) * 32 + 32;
		Vector2 generatePos = Vector2(randomPosX, randomPosY);

		bool blockLeft = false, blockRight = false;
		bool blockTop = false, blockBot = false;

		if (generatePos.x == 48)
		{
			blockLeft = true;
		}

		if (generatePos.x == 208)
		{
			blockRight = true;
		}

		if (generatePos.y == 32)
		{
			blockBot = true;
		}

		if (generatePos.y == 192)
		{
			blockTop = true;
		}

		for (auto map_clone : existingClones)
		{
			auto clone = (CBossZ88*)map_clone.second;
			auto clonePos = clone->GetPosition();
			if (generatePos == clonePos)
			{
				goto randomAgain;
			}

			if (generatePos.x == clonePos.x)
			{
				if (clonePos.y - generatePos.y == 32)
				{
					blockTop = true;
				}

				if (generatePos.y - clonePos.y == 32)
				{
					blockBot = true;
				}
			}

			if (generatePos.y == clonePos.y)
			{
				if (clonePos.x - generatePos.x == 32)
				{
					blockRight = true;
				}

				if (generatePos.x - clonePos.x == 32)
				{
					blockLeft = true;
				}
			}
		}

		auto newClone = Instantiate<CBossZ88>(generatePos);

		if (blockTop == true && blockBot == true && 
			blockLeft == true && blockRight == true)
		{
			newClone->state = Z88State::OnlyShooting;
		}
		else
		{
			if ((blockTop == false || blockBot == false) &&
				(blockLeft == false || blockRight == false))
			{
				if (rand() % 2 == 0)
				{
					if (blockTop == false) newClone->velocity = Vector2(0.0f, MOVING_SPEED);
					else if (blockBot == false) newClone->velocity = Vector2(0.0f, -MOVING_SPEED);
				}
				else
				{
					if (blockLeft == false) newClone->velocity = Vector2(-MOVING_SPEED, 0.0f);
					else if (blockRight == false) newClone->velocity = Vector2(MOVING_SPEED, 0.0f);
				}
			}
			else
			{
				if (blockTop == false || blockBot == false)
				{
					if (blockTop == false) newClone->velocity = Vector2(0.0f, MOVING_SPEED);
					else if (blockBot == false) newClone->velocity = Vector2(0.0f, -MOVING_SPEED);
				}
				else if (blockLeft == false || blockRight == false)
				{
					if (blockLeft == false) newClone->velocity = Vector2(-MOVING_SPEED, 0.0f);
					else if (blockRight == false) newClone->velocity = Vector2(MOVING_SPEED, 0.0f);
				}
			}
		}
	}
}

void CBossZ88::OnlyMoving()
{
}

void CBossZ88::MovingAndShooting()
{
}

void CBossZ88::OnlyShooting()
{
}

CBossZ88::CBossZ88()
{
	InitAnimations();
	InitColliders();

	// Enemy setting
	scrollingMap = false;

	// Static setting
	uncalledCloneCount--;
	existingClones.emplace(std::make_pair(existingClones.size() + 1, this));

	// Self setting
	isSleeping = false;
	tag = ObjectTag::BossZ88;
	velocity = Vector2(MOVING_SPEED, 0.0f);

	lastTimeGenerate = GetTickCount();
}

void CBossZ88::Update(DWORD dt)
{
	if (isSleeping == true) return;
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
		isSleeping = true;
		colliders.at(0)->SetDynamic(false);
		GenerateNewClone();
	}
}
