#pragma once
#include <memory>
#include "GameObject.h"

#define NODE_CAPACITY 25
#define NODE_MAX_DEPTH 6

class CQuadtree
{
private:
	/*static std::vector<CGameObject*> gameObjects;
	static float screen_width, screen_height;*/

	int m_level;
	RectF m_rect;
	std::unique_ptr<CQuadtree> m_subNodes[4];
	std::vector<CGameObject*> m_inNodes;

	void Split();
	void Insert(CGameObject* gameObject);
	bool Contain(CGameObject* gameObject);
	bool ContainRect(const RectF& rect) const;
public:
	//static void SetGameObjects(std::vector<CGameObject*> objects) { gameObjects = objects; }
	//static void SetRect(float scrWidth, float scrHeight) { screen_width = scrWidth; screen_height = scrHeight; }

	CQuadtree(const int level, const RectF& rect);
	~CQuadtree();
	void Update(std::vector<CGameObject*> gameObjects);
	void Retrieve(std::vector<CGameObject*>& container, const RectF& rect);
	void Reset(float screen_width, float screen_height);
};