#include "BossZ88.h"
#include "Animations.h"

int CBossZ88::existingCloneCount = 0;
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
	isSleeping = false;

	existingCloneCount++;
	uncalledCloneCount--;

	existingClones.emplace(std::make_pair(existingCloneCount, this));
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
}

void CBossZ88::GenerateNewClone()
{
	if (uncalledCloneCount > 0)
	{
		randomAgain:

		float randomPosX = CMath::Random(0, 5) * 32 + 48;
		float randomPosY = CMath::Random(0, 5) * 32 + 32;
		auto randomPos = Vector2(randomPosX, randomPosY);

		for (auto map_clone : existingClones)
		{
			auto clone = (CBossZ88*)map_clone.second;
			if (randomPos == clone->GetPosition())
			{
				goto randomAgain;
			}
		}

		Instantiate<CBossZ88>(randomPos);
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

	scrollingMap = false;

	Awaking();

	lastTimeGenerate = GetTickCount();
}

void CBossZ88::Update(DWORD dt)
{
	DWORD now = GetTickCount();
	if (now - lastTimeGenerate > 1000)
	{
		GenerateNewClone();
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
}
