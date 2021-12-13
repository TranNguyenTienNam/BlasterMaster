#include "Sprites.h"
#include "Utils.h"

CSprite::CSprite(int l, int t, int w, int h, LPDIRECT3DTEXTURE9 tex)
{
	this->left = l;
	this->top = t;
	this->width = w;
	this->height = h;
	this->texture = tex;
}

void CSprite::Draw(Vector2 position, int nx, int layer_index, D3DCOLOR color)
{
	CGame::GetInstance()->Draw(position, nx, layer_index, texture, left, top, left + width, top + height, color);
}

void CSprites::Add(std::string id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 texture)
{
	sprites.insert(make_pair(id, new CSprite(left, top, width, height, texture)));
	/*DebugOut(L"[INFO] sprite added: %s\t%d\t%d\t%d\t%d\n", ToWSTR(id).c_str(), left, top, height, width);*/
}

LPSPRITE CSprites::Get(std::string id)
{
	return sprites.at(id);
}

void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}

CMapBackground::CMapBackground(int w, int h, LPDIRECT3DTEXTURE9 tex)
{
	this->width = w;
	this->height = h;
	this->texture = tex;
	this->position = VectorZero();
}

void CMapBackground::Draw(int nx, int layer_index, D3DCOLOR color)
{
	CGame::GetInstance()->Draw(position, nx, layer_index, texture, 0, 0, width, height, color);
}
