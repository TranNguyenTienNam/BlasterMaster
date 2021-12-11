#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Animations.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include "Sophia.h"
#include "Portal.h"
#include "Brick.h"
#include "Jason.h"
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

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

CPlayScene::~CPlayScene()
{
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			1
#define SCENE_SECTION_SPRITES			2
#define SCENE_SECTION_ANIMATIONS		3
#define SCENE_SECTION_MAP				4

#define MAX_SCENE_LINE 1024

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	auto game = CGame::GetInstance();

	// Init Camera
	auto mainCam = game->GetService<CCamera>();
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
		}
	}

	f.close();

	game->GetService<CTextures>()->Add("tex-bbox", L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	game->GetService<CTextures>()->Add("tex-green-bbox", L"textures\\green-bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::_ParseSection_TEXTURES(std::string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	std::string texID = tokens[0].c_str();
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CGame::GetInstance()->GetService<CTextures>()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(std::string line)
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

	CGame::GetInstance()->GetService<CSprites>()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(std::string line)
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

	CGame::GetInstance()->GetService<CAnimations>()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_MAP(std::string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1 || tokens[0] == "") return;

	auto game = CGame::GetInstance();

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
	auto mainCam = game->GetService<CCamera>();

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

			auto textures = game->GetService<CTextures>();
			textures->Add(texID, ToWSTR(image_path).c_str(), D3DCOLOR_XRGB(0, 0, 0));
			auto texmap = textures->Get(texID);

			std::string sprID = "spr-" + image_path;
			sprID.erase(sprID.end() - 4, sprID.end());

			auto sprites = game->GetService<CSprites>();
			sprites->Add(sprID, 0, 0, m_mapWidth, m_mapHeight, texmap);
			map = sprites->Get(sprID);
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
				CGameObject* obj = NULL;

				int x = object["x"].GetInt();
				int y = object["y"].GetInt();

				int width = object["width"].GetInt();
				int height = object["height"].GetInt();

				auto object_type = object["name"].GetString();

				if (strcmp(object_type, "jason") == 0) obj = new CJason;
				else if (strcmp(object_type, "sophia") == 0)
				{
					if (player != NULL)
					{
						DebugOut(L"[ERROR] SOPHIA object was created before!\n");
						return;
					}
					obj = new CSophia;
					player = (CSophia*)obj;
					mainCam->SetTarget(player);

					DebugOut(L"[INFO] Player object created!\n");
				}
				else if (strcmp(object_type, "interrupt") == 0) obj = new CInterrupt;
				else if (strcmp(object_type, "neoworm") == 0) obj = new CNeoworm;
				else if (strcmp(object_type, "ballbot") == 0) obj = new CBallbot;
				else if (strcmp(object_type, "stuka") == 0) obj = new CStuka;
				else if (strcmp(object_type, "eyelet") == 0) obj = new CEyelet;
				else if (strcmp(object_type, "ballcarry") == 0) obj = new CBallCarry;
				else if (strcmp(object_type, "drap") == 0) obj = new CDrap;
				else if (strcmp(object_type, "gx680") == 0) obj = new CGX680;
				else if (strcmp(object_type, "gx680s") == 0) obj = new CGX680S;
				else if (strcmp(object_type, "laserguard") == 0) obj = new CLaserGuard;
				else if (strcmp(object_type, "brick") == 0) obj = new CBrick;
				else if (strcmp(object_type, "portal") == 0)
				{
					int scene_id = 1;
					obj = new CPortal(width, height, scene_id);
					DebugOut(L"[INFO] Portal object created!\n");
				}
				else
				{
					DebugOut(L"[ERR] Invalid object type: %s\n", ToWSTR(object_type).c_str());
					continue;
				}

				if (object.HasMember("properties") == true)
				{
					auto properties = object["properties"].GetArray();
					if (properties.Size() > 0)
					{
						int nx = properties[0]["value"].GetInt();
						obj->SetDirection(nx);
					}
				}

				obj->SetPosition(Vector2(x + width / 2, m_mapHeight - y + height / 2));

				AddGameObject(obj);
			}
		}
	}

	fclose(fp);
}

void CPlayScene::Update(DWORD dt)
{
	auto mainCam = CGame::GetInstance()->GetService<CCamera>();
	mainCam->Update();

	updates.clear();
	quadtree->Update(gameObjects);
	quadtree->Retrieve(updates, mainCam->GetBoundingBox());
	DebugOut(L"[BEGIN LOOP] updates %d\n", updates.size());

	for (auto obj : updates)
		if (obj->IsEnabled() == true) obj->PhysicsUpdate(&updates);

	for (auto obj : updates)
		if (obj->IsEnabled() == true) obj->Update(dt);
}

void CPlayScene::Render()
{
	map->Draw(Vector2(m_mapWidth / 2, m_mapHeight / 2), 1, 1);

	for (auto obj : updates)
		if (obj->IsEnabled() == true) obj->Render();

	// RENDERING GIZMO
	/*for (auto obj : updates)
		if (obj->IsEnabled() == true)
			for (auto co : obj->GetColliders())
				co->RenderBoundingBox();*/
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{ 
	for (auto obj : gameObjects)
		obj->SetDestroyed();
	gameObjects.clear();

	player = NULL;
	
	if (quadtree != nullptr)
	{
		delete quadtree;
		quadtree = nullptr;
	}

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::Clean()
{
	for (auto obj : gameObjects)
	{
		if (obj->IsDestroyed() == true)
		{
			quadtree->RemoveGameObjectFromLeaf(obj);
			delete obj;
		}
	}
}

void CPlayScene::AddGameObject(CGameObject* object)
{
	gameObjects.push_back(object);
	quadtree->Insert(object);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_1:
		CGame::GetInstance()->GetService<CScenes>()->SwitchScene(1);
		break;
	case DIK_2:
		CGame::GetInstance()->GetService<CScenes>()->SwitchScene(2);
		break;
	case DIK_3:
		CGame::GetInstance()->GetService<CScenes>()->SwitchScene(3);
		break;
	default:
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	
}