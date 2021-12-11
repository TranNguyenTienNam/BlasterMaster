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

class CPlayScene : public CScene
{
protected:
	CGameObject* player;
	std::vector<CGameObject*> gameObjects;
	std::vector<CGameObject*> updates;
	CQuadtree* quadtree;					// Quadtree for space partitioning

	float m_mapWidth;
	float m_mapHeight;

	LPSPRITE map;

	void _ParseSection_TEXTURES(std::string line);
	void _ParseSection_SPRITES(std::string line);
	void _ParseSection_ANIMATIONS(std::string line);
	void _ParseSection_MAP(std::string line);

public:
	CPlayScene(int id, LPCWSTR filePath);
	~CPlayScene();

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void Clean();

	CGameObject* GetPlayer() { return player; }
	void SetPlayer(CGameObject* object) { this->player = object; }
	void AddGameObject(CGameObject* object);
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
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