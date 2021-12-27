#pragma once

enum class ObjectTag
{
	Player,
	Sophia,
	Jason,
	BigJason,

	Enemy,
	BossZ88,

	Item,

	PlayerBullet,
	EnemyBullet,

	Platform,
	HarmPlatform,

	Portal,
	MiniPortal,
	BossTrigger,

	Unknown,
};

class TagUtils
{
	// Get groups of object tag for purposes
public:
	static bool PlayerTag(ObjectTag tag);
	static bool EnemyTag(ObjectTag tag);
};

