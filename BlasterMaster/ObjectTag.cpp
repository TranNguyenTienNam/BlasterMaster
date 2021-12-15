#include "ObjectTag.h"

bool TagUtils::PlayerTag(ObjectTag tag)
{
	return tag == ObjectTag::Jason || tag == ObjectTag::Sophia || 
		tag == ObjectTag::BigJason || tag == ObjectTag::Player;
}
