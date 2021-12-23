#pragma once

enum class ObjectTag
{
	Player,
	Sophia,
	Jason,
	BigJason,

	Enemy,
	Eyelet,

	Item,

	PlayerBullet,
	EnemyBullet,

	Platform,
	HarmPlatform,

	Portal,
	MiniPortal,
	BossTrigger,
};

class TagUtils
{
	// Get groups of object tag for purposes
public:
	static bool PlayerTag(ObjectTag tag);
};

