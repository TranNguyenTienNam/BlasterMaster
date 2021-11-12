#include "Game.h"
#include "Animations.h"
#include "Utils.h"
#include "InputHandler.h"
#include "Drap.h"
#include "Playable.h"
#include "Camera.h"
#include "Quadtree.h"

CGame* CGame::instance = NULL;
DWORD CGame::deltaTime = 0;

CPlayable* playable;
std::vector<CGameObject*> gameObjects;
std::vector<CGameObject*> updates;

CCamera* mainCam;

CQuadtree* quadtree;

class CSampleKeyHandler : public CKeyEventHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	auto handler = CGame::GetInstance()->GetService<CInputHandler>();
	if (handler->IsKeyDown(DIK_RIGHT))
		playable->SetState(PLAYER_STATE_WALKING_RIGHT);
	else if (handler->IsKeyDown(DIK_LEFT))
		playable->SetState(PLAYER_STATE_WALKING_LEFT);
	else if (handler->IsKeyDown(DIK_UP))
		playable->SetState(PLAYER_STATE_WALKING_TOP);
	else if (handler->IsKeyDown(DIK_DOWN))
		playable->SetState(PLAYER_STATE_WALKING_DOWN);
	else playable->SetState(DRAP_STATE_IDLE);
}

CGame* CGame::GetInstance()
{
	if (instance == NULL) instance = new CGame();
	return instance;
}

void CGame::InitDirectX(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = NULL;

	// retrieve window width & height so that we can create backbuffer height & width accordingly 
	RECT r;
	GetClientRect(hWnd, &r);

	screen_width = r.right;
	screen_height = r.bottom;

	d3dpp.BackBufferHeight = screen_height;
	d3dpp.BackBufferWidth = screen_width;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,			// use default video card in the system, some systems have more than one video cards
		D3DDEVTYPE_HAL,				// HAL = Hardware Abstraction Layer - a "thin" software layer to allow application to directly interact with video card hardware
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		DebugOut(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite handler from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);
	
	if (spriteHandler == NULL)
	{
		DebugOut(L"[ERROR] Create spriteHandler failed\n");
		return;
	}

	DebugOut(L"[INFO] InitDirectX OK\n");
}

void CGame::Draw(Vector2 position, int nx, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{
	Vector2 camPos = mainCam->GetPosition();
	Vector3 p = Vector3(0, 0, 0);

	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;

	Vector3 center = Vector3((right - left) / 2, (bottom - top) / 2, 0.0f);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	// FlipX
	D3DXMATRIX flipX;
	D3DXMatrixScaling(&flipX, -nx, 1.0f, 1.0f);

	// Translate
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, (position.x - camPos.x), (-position.y + camPos.y), 0.0f);

	mat *= flipX;
	mat *= translate;

	spriteHandler->SetTransform(&mat);

	spriteHandler->Draw(texture, &r, &center, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

LPDIRECT3DTEXTURE9 CGame::LoadTexture(LPCWSTR texturePath, D3DCOLOR transparentColor)
{
	LPDIRECT3DTEXTURE9 texture;

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();

	HRESULT result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		texturePath,						// Path to the image to load
		D3DX_DEFAULT_NONPOW2, 				// Auto texture width (get from file)
		D3DX_DEFAULT_NONPOW2, 				// Auto texture height (get from file)
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,		// Transparent color
		NULL,
		NULL,
		&texture);							// Created texture pointer

	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] CreateTextureFromFileEx %s failed\n", texturePath);
		return NULL;
	}

	DebugOut(L"[INFO] Texture loaded Ok: %s \n", texturePath);
	return texture;
}

void CGame::Update(DWORD dt)
{
	mainCam->Update();

	updates.clear();
	quadtree->Reset(screen_width * 10, screen_height * 10);
	quadtree->Update(gameObjects);
	quadtree->Retrieve(updates, mainCam->GetBoundingBox());
	DebugOut(L"updates %d\n", updates.size());

	for (auto obj : updates)
		obj->Update(dt);
}

void CGame::Render()
{
	if (d3ddv->BeginScene() == D3D_OK)
	{
		// Clear the whole window with a color
		d3ddv->ColorFill(GetBackBuffer(), NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		for (auto obj : updates)
			obj->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

void CGame::GameInit(HWND hWnd)
{
	InitDirectX(hWnd);

	AddService(new CTextures);
	GetService<CTextures>()->Add("tex-enemies", L"textures\\enemies.png", D3DCOLOR_XRGB(255, 255, 255));
	GetService<CTextures>()->Add("tex-player", L"textures\\player.png", D3DCOLOR_XRGB(0, 57, 115));

	AddService(new CSprites);
	GetService<CSprites>()->Add("spr-drap-0", 128, 274, 18, 18, GetService<CTextures>()->Get("tex-enemies"));
	GetService<CSprites>()->Add("spr-drap-1", 148, 274, 18, 18, GetService<CTextures>()->Get("tex-enemies"));
	GetService<CSprites>()->Add("spr-drap-3", 168, 274, 18, 18, GetService<CTextures>()->Get("tex-enemies"));
	GetService<CSprites>()->Add("spr-drap-2", 188, 274, 18, 18, GetService<CTextures>()->Get("tex-enemies"));

	GetService<CSprites>()->Add("spr-jason-walk-jump", 12, 30, 8, 16, GetService<CTextures>()->Get("tex-player"));
	GetService<CSprites>()->Add("spr-jason-walk-0", 21, 30, 8, 16, GetService<CTextures>()->Get("tex-player"));
	GetService<CSprites>()->Add("spr-jason-walk-1", 30, 30, 8, 16, GetService<CTextures>()->Get("tex-player"));

	AddService(new CAnimations);
	auto anim = new CAnimation;
	anim->Add("spr-drap-0", 200);
	anim->Add("spr-drap-1", 200);
	anim->Add("spr-drap-2", 200);
	anim->Add("spr-drap-3", 200);
	GetService<CAnimations>()->Add("ani-drap", anim);

	anim = new CAnimation;
	anim->Add("spr-jason-walk-jump", 100);
	anim->Add("spr-jason-walk-0", 100);
	anim->Add("spr-jason-walk-1", 100);
	GetService<CAnimations>()->Add("ani-jason", anim);

	AddService(new CInputHandler);
	GetService<CInputHandler>()->SetHandleWindow(hWnd);

	key_handler = new CSampleKeyHandler;
	GetService<CInputHandler>()->SetKeyHandler(key_handler);
	GetService<CInputHandler>()->Initialize();

	// Instantiate game objects
	auto obj = new CPlayable;
	obj->SetPosition(Vector2(100, 100));
	obj->SetSpeed(Vector2(0, 0));
	gameObjects.push_back(obj);
	playable = obj;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++)
		{
			auto obj1 = new CDrap;
			obj1->SetPosition(Vector2(i * 100, j * 100));
			obj1->SetSpeed(Vector2(0, 0));
			gameObjects.push_back(obj1);
		}
	}
	
	mainCam = new CCamera;
	mainCam->SetTarget(playable);
	mainCam->SetBoundingBoxSize(Vector2(screen_width, screen_height));
	DebugOut(L"scr %d %d\n", screen_width, screen_height);

	quadtree = new CQuadtree(0, RectF(0, 0, screen_width * 10, screen_height * 10));
	/*quadtree->SetGameObjects(gameObjects);
	quadtree->SetRect(screen_width, screen_height);*/
}

void CGame::GameRun()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		deltaTime = now - frameStart;

		if (deltaTime >= tickPerFrame)
		{
			frameStart = now;

			CGame::GetInstance()->GetService<CInputHandler>()->ProcessKeyboard();

			Update(deltaTime);
			Render();
		}
		else
			Sleep(tickPerFrame - deltaTime);
	}
}

void CGame::GameEnd()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL)backBuffer->Release();
	if (d3ddv != NULL)d3ddv->Release();
	if (d3d != NULL) d3d->Release();
	DebugOut(L"[INFO] Cleanup Ok\n");

	quadtree->Reset(screen_width, screen_height);
}