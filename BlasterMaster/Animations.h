#pragma once
#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "Animation.h"

class CAnimations : public CService
{
	std::unordered_map<std::string, LPANIMATION> animations;

public:
	void Add(std::string id, LPANIMATION anim);
	LPANIMATION Get(std::string id);
	LPANIMATION Clone(std::string id);
	void Clear();
};

#endif