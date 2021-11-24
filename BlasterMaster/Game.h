#pragma once
#ifndef GAME_H
#define GAME_H

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "ServiceLocator.h"
#include "Transform.h"
#include "KeyEventHandler.h"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)

#define MAX_FRAME_RATE 120

class CGameObject;
class CTile;

class CGame : public CServiceLocator
{
	static CGame* instance;
	static DWORD deltaTime;						// The interval in seconds from the last frame to the current one

	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D images 

	CKeyEventHandler* key_handler;

	int screen_width;
	int screen_height;

	float m_mapWidth;
	float m_mapHeight;

	std::vector<CTile*> tilemap;
public:
	static CGame* GetInstance();
	static DWORD GetDeltaTime() { return deltaTime; }

	void InitDirectX(HWND hWnd);

	void Draw(Vector2 position, int nx, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha);
	LPDIRECT3DTEXTURE9 LoadTexture(LPCWSTR texturePath, D3DCOLOR transparentColor);

	void Load();
	void _ParseSection_TEXTURES(std::string line);
	void _ParseSection_SPRITES(std::string line);
	void _ParseSection_ANIMATIONS(std::string line);
	void _ParseSection_TILEMAP(std::string line);
	void _ParseSection_OBJECTS(std::string line);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }
	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	void Update(DWORD dt);
	void Render();

	void GameInit(HWND hWnd);
	void GameRun();
	void GameEnd();
};

#endif 