#pragma once
#include <memory>

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

	LPMAPBACKGROUND map;								// current scene
	std::vector<CGameObject*> gameObjects;				//
	std::vector<CGameObject*> requests, destroyed;
	
	LPMAPBACKGROUND map_switching;						// TODO: last scene, maybe both vector?
	std::vector<CGameObject*> gameObjects_switching;	//

	std::vector<CGameObject*> updates;
	CQuadtree* quadtree;					// Quadtree for space partitioning

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
	virtual void PreSwitchingSection(std::vector<CGameObject*> objects, LPMAPBACKGROUND mapBackGround);
	virtual void AfterSwitchingSection();

	virtual void HandlingInstantiateRequest();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void Clean();

	void SetState(PlaySceneState _state) { this->state = _state; }
	LPMAPBACKGROUND GetMapBackground() { return this->map; }
	CGameObject* GetPlayer() { return player; }
	void SetPlayer(CGameObject* object) { this->player = object; }
	std::vector<CGameObject*> GetGameObjects() { return this->gameObjects; }
	void AddGameObject(CGameObject* object);
	void RequestInstantiate(CGameObject* object);
};

template<typename T>
inline T* Instantiate(Vector2 position)
{
	CGameObject* newObject = new T;
	newObject->SetPosition(position);
	auto current_scene = (CPlayScene*)CGame::GetInstance()->GetService<CScenes>()->GetCurrentScene();
	current_scene->AddGameObject(newObject);
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