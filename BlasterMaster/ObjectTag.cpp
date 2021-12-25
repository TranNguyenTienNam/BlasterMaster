#include "ObjectTag.h"

bool TagUtils::PlayerTag(ObjectTag tag)
{
	return tag == ObjectTag::Jason || tag == ObjectTag::Sophia || 
		tag == ObjectTag::BigJason || tag == ObjectTag::Player;
}

bool TagUtils::EnemyTag(ObjectTag tag)
{
	return tag == ObjectTag::Enemy || tag == ObjectTag::BossZ88;
}
