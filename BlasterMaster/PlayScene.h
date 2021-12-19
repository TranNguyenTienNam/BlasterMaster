#pragma once
#include <memory>
#include <unordered_map>

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Sprites.h"
#include "Quadtree.h"
#include "Scenes.h"

const int CELL_SIZE = 128;

class CGameObject;
class CPortal;
class CQuadtree;

enum class PlaySceneState
{
	FreePlaying,
	Switching,
};

class CPlayScene : public CScene
{
protected:
	PlaySceneState state;

	CGameObject* player;
	std::unordered_map<int, CPortal*> portals;

	LPMAPSPRITE background;								// current scene
	LPMAPSPRITE foreground;
	std::vector<CGameObject*> gameObjects;				//
	std::vector<CGameObject*> instantiateds, destroyeds;

	std::vector<CGameObject*> updates;
	CQuadtree* quadtree;					// Quadtree for space partitioning
	
	LPMAPSPRITE background_switching;
	LPMAPSPRITE foreground_switching;
	std::vector<CGameObject*> gameObjects_switching;	

	float m_mapWidth;
	float m_mapHeight;

	void _ParseSection_TEXTURES(std::string line);
	void _ParseSection_SPRITES(std::string line);
	void _ParseSection_ANIMATIONS(std::string line);
	void _ParseSection_MAP(std::string line);

public:
	CPlayScene(int id, LPCWSTR filePath);
	~CPlayScene();

	virtual void Load();
	virtual void PreSwitchingSection(CPlayScene* lastScene, Vector2 translation);
	virtual void AfterSwitchingSection();

	virtual void HandlingInstantiateRequest();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void Clean();

	void SetState(PlaySceneState _state) { this->state = _state; }
	LPMAPSPRITE GetMapBackground() { return this->background; }
	CGameObject* GetPlayer() { return player; }
	std::unordered_map<int, CPortal*> GetPortalList(){ return this->portals; }
	void SetPlayer(CGameObject* object) { this->player = object; }
	std::vector<CGameObject*> GetUpdateObjects() { return this->updates; }
	
	void AddGameObject(CGameObject* object);
	void RequestInstantiate(CGameObject* object);
};

template<typename T>
inline T* Instantiate(Vector2 position)
{
	CGameObject* newObject = new T;
	newObject->SetPosition(position);
	auto current_scene = (CPlayScene*)CGame::GetInstance()->GetService<CScenes>()->GetCurrentScene();
	current_scene->RequestInstantiate(newObject);
	return (T*)newObject;
}

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};