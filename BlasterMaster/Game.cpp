#include "Game.h"
#include "Animations.h"
#include "Utils.h"
#include "InputHandler.h"
#include "Drap.h"
#include "Playable.h"
#include "Camera.h"
#include "Quadtree.h"
#include "Sophia.h"
#include "Jason.h"
#include "Brick.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

CGame* CGame::instance = NULL;
DWORD CGame::deltaTime = 0;

CJason* jason;
CSophia* sophia;
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
	switch (KeyCode)
	{
	case DIK_LSHIFT:
		if (jason->controllable == true) 
		{
			jason->controllable = false;
			sophia->controllable = true;
			mainCam->SetTarget(sophia);
		}
		else
		{
			jason->controllable = true;
			sophia->controllable = false;
			mainCam->SetTarget(jason);
		}
	default:
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	
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
	D3DXMatrixScaling(&flipX, nx, 1.0f, 1.0f);

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

void CGame::LoadTileMap(std::string filePath)
{
	FILE* fp;
	errno_t err = fopen_s(&fp, filePath.c_str(), "r");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	int tileWidth = d["tilewidth"].GetInt();
	int tileHeight = d["tileheight"].GetInt();

	int mapWidth = d["width"].GetInt();			// Calculate by tile
	int mapHeight = d["height"].GetInt();

	// Set boundary of camera
	RectF boundary;
	boundary.left = -tileWidth / 2;
	boundary.top = mapHeight * tileHeight + tileHeight / 2;
	boundary.right = mapWidth * tileWidth - tileWidth / 2;
	boundary.bottom = tileHeight / 2;
	mainCam->GetBoundary(boundary);

	// Init Grid
	m_mapWidth = mapWidth * tileWidth;
	m_mapHeight = mapHeight * tileHeight;
	quadtree = new CQuadtree(0, RectF(0, m_mapHeight, m_mapWidth, 0));
	quadtree->Reset(m_mapWidth, m_mapHeight);

	// Tileset texture settings
	int columns = d["tilesets"].GetArray()[0]["columns"].GetInt();
	int spacing = d["tilesets"].GetArray()[0]["spacing"].GetInt();
	/*auto image_path = ToWSTR(d["tilesets"].GetArray()[0]["image"].GetString());*/

	/*CGame::GetInstance()->GetService<CTextures>()->Add("tex-tileset", image_path.c_str(), D3DCOLOR_XRGB(0, 0, 0));*/

	auto layers = d["layers"].GetArray();

	for (auto& layer : layers)
	{
		auto layer_type = layer["type"].GetString();
		auto visible = layer["visible"].GetBool();

		// Tile Layer
		if (strcmp(layer_type, "tilelayer") == 0 && visible == true)
		{
			auto data = layer["data"].GetArray();

			for (int x = 0; x < mapWidth; x++)
			{
				for (int y = 0; y < mapHeight; y++)
				{
					int tilesetID = data[y * mapWidth + x].GetInt() - 1;

					// Get tile coordinate in tileset by id
					int tileX = tilesetID % columns;
					int tileY = tilesetID / columns;

					int left = tileX * (tileWidth + spacing);
					int top = tileY * (tileHeight + spacing);

					auto texTileset = CGame::GetInstance()->GetService<CTextures>()->Get("tex-tileset");

					int posX = x * tileWidth;
					int posY = (mapHeight - y) * tileHeight;

					Vector2 position = Vector2(posX, posY);

					auto newTile = new CTile(position, left, top, tileWidth, tileHeight, texTileset);

					tilemap.push_back(newTile);
				}
			}
		}
		// Object Layer
		else if (strcmp(layer_type, "objectgroup") == 0 && visible == true)
		{
			auto objects = layer["objects"].GetArray();

			for (int i = 0; i < objects.Size(); i++)
			{
				CGameObject* obj = new CBrick;
				int x = objects[i]["x"].GetInt();
				int y = objects[i]["y"].GetInt();

				obj->SetPosition(Vector2(x, m_mapHeight - y + 16));	// TODO: CONST

				gameObjects.push_back(obj);
				quadtree->Insert(obj);
			}
		}
	}

	fclose(fp);
}

void CGame::Update(DWORD dt)
{
	mainCam->Update();

	updates.clear();
	quadtree->Update(gameObjects);
	quadtree->Retrieve(updates, mainCam->GetBoundingBox());
	DebugOut(L"updates %d\n", updates.size());

	for (auto obj : updates)
		obj->PhysicsUpdate(&updates);

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

		for (auto tile : tilemap)
			tile->Draw(255);

		for (auto obj : updates)
			obj->Render();

		/*for (auto obj : updates)
			for (auto co : obj->GetColliders())
				co->RenderBoundingBox();*/

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
	GetService<CTextures>()->Add("tex-bbox", L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	GetService<CTextures>()->Add("tex-enemies", L"textures\\enemies.png", D3DCOLOR_XRGB(41, 255, 4));
	GetService<CTextures>()->Add("tex-player", L"textures\\player.png", D3DCOLOR_XRGB(41, 255, 4));
	GetService<CTextures>()->Add("tex-tileset", L"textures\\tileset.png", D3DCOLOR_XRGB(0, 0, 0));

	AddService(new CSprites);
	GetService<CSprites>()->Add("spr-drap-0", 128, 274, 18, 18, GetService<CTextures>()->Get("tex-enemies"));
	GetService<CSprites>()->Add("spr-drap-1", 148, 274, 18, 18, GetService<CTextures>()->Get("tex-enemies"));
	GetService<CSprites>()->Add("spr-drap-3", 168, 274, 18, 18, GetService<CTextures>()->Get("tex-enemies"));
	GetService<CSprites>()->Add("spr-drap-2", 188, 274, 18, 18, GetService<CTextures>()->Get("tex-enemies"));

	// JASON
	GetService<CSprites>()->Add("spr-jason-idle", 3, 30, 8, 16, GetService<CTextures>()->Get("tex-player"));

	GetService<CSprites>()->Add("spr-jason-walk-jump", 12, 30, 8, 16, GetService<CTextures>()->Get("tex-player"));
	GetService<CSprites>()->Add("spr-jason-walk-0", 21, 30, 8, 16, GetService<CTextures>()->Get("tex-player"));
	GetService<CSprites>()->Add("spr-jason-walk-1", 30, 30, 8, 16, GetService<CTextures>()->Get("tex-player"));

	// SOPHIA
	GetService<CSprites>()->Add("spr-sophia-wheel-0", 3, 21, 8, 8, GetService<CTextures>()->Get("tex-player"));
	GetService<CSprites>()->Add("spr-sophia-wheel-1", 12, 21, 8, 8, GetService<CTextures>()->Get("tex-player"));
	GetService<CSprites>()->Add("spr-sophia-wheel-2", 21, 21, 8, 8, GetService<CTextures>()->Get("tex-player"));
	GetService<CSprites>()->Add("spr-sophia-wheel-3", 30, 21, 8, 8, GetService<CTextures>()->Get("tex-player"));

	GetService<CSprites>()->Add("spr-sophia-middle", 3, 12, 8, 8, GetService<CTextures>()->Get("tex-player"));

	GetService<CSprites>()->Add("spr-sophia-gun-00", 12, 3, 8, 8, GetService<CTextures>()->Get("tex-player"));
	GetService<CSprites>()->Add("spr-sophia-gun-45", 21, 3, 8, 8, GetService<CTextures>()->Get("tex-player"));
	GetService<CSprites>()->Add("spr-sophia-gun-90", 30, 3, 8, 8, GetService<CTextures>()->Get("tex-player"));

	GetService<CSprites>()->Add("spr-sophia-cabin", 39, 3, 16, 8, GetService<CTextures>()->Get("tex-player"));
	GetService<CSprites>()->Add("spr-sophia-cabin-turn", 56, 3, 16, 8, GetService<CTextures>()->Get("tex-player"));
	GetService<CSprites>()->Add("spr-sophia-cabin-45", 73, 3, 16, 16, GetService<CTextures>()->Get("tex-player"));

	AddService(new CAnimations);
	auto anim = new CAnimation;
	anim->Add("spr-drap-0", 150);
	anim->Add("spr-drap-1", 150);
	anim->Add("spr-drap-2", 150);
	anim->Add("spr-drap-3", 150);
	GetService<CAnimations>()->Add("ani-drap", anim);

	anim = new CAnimation;
	anim->Add("spr-jason-idle", 100);
	GetService<CAnimations>()->Add("ani-jason-idle", anim);

	anim = new CAnimation;
	anim->Add("spr-jason-walk-jump", 100);
	anim->Add("spr-jason-walk-0", 100);
	anim->Add("spr-jason-walk-1", 100);
	GetService<CAnimations>()->Add("ani-jason-walk", anim);

	anim = new CAnimation;
	anim->Add("spr-jason-walk-jump", 100);
	GetService<CAnimations>()->Add("ani-jason-jump", anim);

	anim = new CAnimation;
	anim->Add("spr-sophia-wheel-0", 100);
	anim->Add("spr-sophia-wheel-1", 100);
	anim->Add("spr-sophia-wheel-2", 100);
	anim->Add("spr-sophia-wheel-3", 100);
	GetService<CAnimations>()->Add("ani-sophia-left-wheel", anim);

	anim = new CAnimation;
	anim->Add("spr-sophia-wheel-3", 100);
	anim->Add("spr-sophia-wheel-0", 100);
	anim->Add("spr-sophia-wheel-1", 100);
	anim->Add("spr-sophia-wheel-2", 100);
	GetService<CAnimations>()->Add("ani-sophia-right-wheel", anim);

	AddService(new CInputHandler);
	GetService<CInputHandler>()->SetHandleWindow(hWnd);

	key_handler = new CSampleKeyHandler;
	GetService<CInputHandler>()->SetKeyHandler(key_handler);
	GetService<CInputHandler>()->Initialize();

	mainCam = new CCamera;
	mainCam->SetBoundingBoxSize(Vector2(screen_width, screen_height));

	LoadTileMap("maps/sectionA.json");

	// Instantiate game objects
	sophia = new CSophia;
	sophia->SetPosition(Vector2(88, 350));
	sophia->SetControllable(false);
	gameObjects.push_back(sophia);
	quadtree->Insert(sophia);

	jason = new CJason;
	jason->SetPosition(Vector2(68, 400));
	jason->SetControllable(true);
	gameObjects.push_back(jason);
	quadtree->Insert(jason);
	mainCam->SetTarget(jason);

	/*for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++)
		{
			auto obj = new CDrap;
			obj->SetPosition(Vector2(i * 50, j * 50));
			gameObjects.push_back(obj);
		}
	}*/
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