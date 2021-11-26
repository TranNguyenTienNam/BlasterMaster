#include <fstream>

#include "Game.h"
#include "Animations.h"
#include "Utils.h"
#include "InputHandler.h"
#include "Playable.h"
#include "Camera.h"
#include "Quadtree.h"

#include "Sophia.h"
#include "Jason.h"
#include "Brick.h"
#include "Interrupt.h"
#include "Neoworm.h"
#include "Ballbot.h"
#include "Stuka.h"
#include "Eyelet.h"
#include "BallCarry.h"
#include "Drap.h"
#include "GX680.h"
#include "GX680S.h"
#include "LaserGuard.h"

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

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			1
#define SCENE_SECTION_SPRITES			2
#define SCENE_SECTION_ANIMATIONS		3
#define SCENE_SECTION_MAP				4
#define SCENE_SECTION_OBJECTS			5

#define MAX_SCENE_LINE 1024

void CGame::Load()
{
	LPCWSTR sceneFilePath = L"database\\scene2.txt";
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	auto game = CGame::GetInstance();

	// Init Camera
	mainCam = new CCamera();
	mainCam->SetBoundingBoxSize(Vector2(game->GetScreenWidth(), game->GetScreenHeight()));

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[TILEMAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	game->GetService<CTextures>()->Add("tex-bbox", L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	game->GetService<CTextures>()->Add("tex-green-bbox", L"textures\\green-bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CGame::_ParseSection_TEXTURES(std::string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	std::string texID = tokens[0].c_str();
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	GetService<CTextures>()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CGame::_ParseSection_SPRITES(std::string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	std::string ID = tokens[0].c_str();
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	std::string texID = tokens[5].c_str();

	LPDIRECT3DTEXTURE9 tex = CGame::GetInstance()->GetService<CTextures>()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	GetService<CSprites>()->Add(ID, l, t, r, b, tex);
}

void CGame::_ParseSection_ANIMATIONS(std::string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	std::string ani_id = tokens[0].c_str();

	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		std::string sprite_id = tokens[i].c_str();
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	GetService<CAnimations>()->Add(ani_id, ani);
}

void CGame::_ParseSection_MAP(std::string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1 || tokens[0] == "") return;

	string filePath = tokens[0];

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
	boundary.left = 0;
	boundary.top = mapHeight * tileHeight;
	boundary.right = mapWidth * tileWidth;
	boundary.bottom = 0;
	mainCam->GetBoundary(boundary);

	// Init Quadtree
	m_mapWidth = mapWidth * tileWidth;
	m_mapHeight = mapHeight * tileHeight;
	quadtree = new CQuadtree(0, RectF(0, m_mapHeight, m_mapWidth, 0));
	quadtree->Reset(m_mapWidth, m_mapHeight);

	// Set map texture
	auto properties = d["properties"].GetArray();

	for (auto& prop : properties)
	{
		if (strcmp(prop["name"].GetString(), "Image Path") == 0)
		{
			std::string image_path = prop["value"].GetString();
			std::string texID = "tex-" + image_path;
			texID.erase(texID.end() - 4, texID.end());
			GetService<CTextures>()->Add(texID, ToWSTR(image_path).c_str(), D3DCOLOR_XRGB(0, 0, 0));
			auto texmap = GetService<CTextures>()->Get(texID);

			std::string sprID = "spr-" + image_path;
			sprID.erase(sprID.end() - 4, sprID.end());
			GetService<CSprites>()->Add(sprID, 0, 0, m_mapWidth, m_mapHeight, texmap);
			map = GetService<CSprites>()->Get(sprID);
		}
	}

	// Insert platform objects
	auto layers = d["layers"].GetArray();

	for (auto& layer : layers)
	{
		auto layer_type = layer["type"].GetString();
		auto visible = layer["visible"].GetBool();

		// Object Layer
		if (strcmp(layer_type, "objectgroup") == 0 && visible == true)
		{
			auto objects = layer["objects"].GetArray();

			for (auto& object : objects)
			{
				CGameObject* obj = new CBrick;
				int x = object["x"].GetInt();
				int y = object["y"].GetInt();

				obj->SetPosition(Vector2(x + 8, m_mapHeight - y + 8));	// TODO: CONST

				gameObjects.push_back(obj);
				quadtree->Insert(obj);
			}
		}
	}

	fclose(fp);
}

void CGame::_ParseSection_OBJECTS(std::string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	string object_type = tokens[0].c_str();
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	Vector2 pos = Vector2(x, y);

	CGameObject* obj = NULL;

	if (object_type == "obj-jason")
	{
		obj = new CJason;
		jason = (CJason*)obj;
	}
	else if (object_type == "obj-sophia")
	{
		obj = new CSophia;
		sophia = (CSophia*)obj;
		mainCam->SetTarget(sophia);
	}
	else if (object_type == "obj-interrupt") obj = new CInterrupt;
	else if (object_type == "obj-neoworm") obj = new CNeoworm;
	else if (object_type == "obj-ballbot") obj = new CBallbot;
	else if (object_type == "obj-stuka") obj = new CStuka;
	else if (object_type == "obj-eyelet") obj = new CEyelet;
	else if (object_type == "obj-ballcarry") obj = new CBallCarry;
	else if (object_type == "obj-drap") obj = new CDrap;
	else if (object_type == "obj-gx680") obj = new CGX680;
	else if (object_type == "obj-gx680s") obj = new CGX680S;
	else if (object_type == "obj-laserguard") obj = new CLaserGuard;
	else
	{
		DebugOut(L"[ERR] Invalid object type: %s\n", ToWSTR(object_type).c_str());
		return;
	}

	// General object setup
	obj->SetPosition(pos);
	gameObjects.push_back(obj);

	quadtree->Insert(obj);
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

		map->Draw(Vector2(m_mapWidth / 2, m_mapHeight / 2), 1, 255);

		for (auto obj : updates)
			obj->Render();

		for (auto obj : updates)
			for (auto co : obj->GetColliders())
				co->RenderBoundingBox();

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
	AddService(new CSprites);
	AddService(new CAnimations);
	AddService(new CInputHandler);
	key_handler = new CSampleKeyHandler();
	GetService<CInputHandler>()->SetKeyHandler(key_handler);
	GetService<CInputHandler>()->SetHandleWindow(hWnd);
	GetService<CInputHandler>()->Initialize();

	Load();
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