#pragma once
#ifndef SPRITES_H
#define SPRITES_H

#include <unordered_map>
#include "Textures.h"

class CSprite
{
protected:
	int left;
	int top;
	int width;
	int height;

	LPDIRECT3DTEXTURE9 texture;

public:
	CSprite(int left, int top, int witdh, int height, LPDIRECT3DTEXTURE9 texture);

	void Draw(Vector2 position, int nx, int layer_index, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));
};

typedef CSprite* LPSPRITE;

class CSprites : public CService
{
	std::unordered_map<std::string, LPSPRITE> sprites;

public:
	void Add(std::string id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 texture);
	LPSPRITE Get(std::string id);
	void Clear();
};

class CMapBackground : public CSprite
{
protected:
	Vector2 position;
public:
	CMapBackground();
	Vector2 GetPosition() { return this->position; }
	void SetPosition(Vector2 pos) { this->position = pos; }
	void Draw(int nx, int layer_index, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));
};

typedef CMapBackground* LPMAPBACKGROUND;
#endif