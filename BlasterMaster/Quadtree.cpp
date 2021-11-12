#include "Quadtree.h"
#include "Utils.h"

CQuadtree::CQuadtree(const int level, const RectF& rect)
	: m_level(level), m_rect(rect), m_subNodes{ nullptr, nullptr, nullptr, nullptr }
{
	m_inNodes.reserve(NODE_CAPACITY);
}

CQuadtree::~CQuadtree()
{
}

void CQuadtree::Update(std::vector<CGameObject*> gameObjects)
{
	m_inNodes.clear();
	m_inNodes.shrink_to_fit();

	m_subNodes[0] = nullptr;
	m_subNodes[1] = nullptr;
	m_subNodes[2] = nullptr;
	m_subNodes[3] = nullptr;

	for (const auto& object : gameObjects) {
		Insert(object);
	}
}

void CQuadtree::Split()
{
	DebugOut(L"Rect %f %f %f %f level %d\n", m_rect.left, m_rect.top, m_rect.right, m_rect.bottom, m_level);

	Vector2 min = Vector2(m_rect.left, m_rect.bottom);
	Vector2 max = Vector2(m_rect.right, m_rect.top);

	float x = min.x;
	float y = min.y;
	float width = max.x - min.x;
	float height = max.y - min.y;

	float w = width * 0.5;
	float h = height * 0.5;

	RectF left_bottom = RectF(x, y, x + w, y + h);
	RectF right_bottom = RectF(x + w, y, x + width, y + h);
	RectF left_top = RectF(x, y + h, x + w, y + height);
	RectF right_top = RectF(x + w, y + h, x + width, y + height);

	m_subNodes[0] = std::make_unique<CQuadtree>(m_level + 1, left_bottom);
	m_subNodes[1] = std::make_unique<CQuadtree>(m_level + 1, right_bottom);
	m_subNodes[2] = std::make_unique<CQuadtree>(m_level + 1, left_top);
	m_subNodes[3] = std::make_unique<CQuadtree>(m_level + 1, right_top);
}

void CQuadtree::Insert(CGameObject* gameObject)
{
	// If this node has split
	if (m_subNodes[0] != nullptr) {
		// Find the subnodes that contain it and insert it there
		if (m_subNodes[0]->Contain(gameObject)) m_subNodes[0]->Insert(gameObject);
		if (m_subNodes[1]->Contain(gameObject)) m_subNodes[1]->Insert(gameObject);
		if (m_subNodes[2]->Contain(gameObject)) m_subNodes[2]->Insert(gameObject);
		if (m_subNodes[3]->Contain(gameObject)) m_subNodes[3]->Insert(gameObject);

		/*DebugOut(L"Insert splited obj %f %f level %d\n", gameObject->GetPosition().x, gameObject->GetPosition().y, m_level);*/

		return;
	}

	// Add object to this node
	m_inNodes.emplace_back(gameObject);

	// If it has NOT split and NODE_CAPACITY is reached and we are not at MAX LEVEL
	if (m_inNodes.size() > NODE_CAPACITY && m_level < NODE_MAX_DEPTH) {
		/*DebugOut(L"Insert not split obj %f %f level %d\n", gameObject->GetPosition().x, gameObject->GetPosition().y, m_level);*/
		// Split into subnodes
		Split();

		// Go through all this nodes objects
		for (const auto& obj : m_inNodes)
		{
			// Go through all newly created subnodes
			for (const auto& subnode : m_subNodes) {
				// If they contain the objects
				if (subnode->Contain(obj)) {
					// Insert the object into the subnode
					subnode->Insert(obj);
				}
			}
		}

		// Remove all indexes from THIS node
		m_inNodes.clear();
		m_inNodes.shrink_to_fit();
	}
}

void CQuadtree::Retrieve(std::vector<CGameObject*>& container, const RectF& rect)
{
	/*DebugOut(L"Rect %f %f %f %f level %d\n", m_rect.left, m_rect.top, m_rect.right, m_rect.bottom, m_level);*/
	if (m_subNodes[0] != nullptr)
	{
		if (m_subNodes[0]->ContainRect(rect))
		{
			/*DebugOut(L"Rect 0 %f %f %f %f level %d\n", m_rect.left, m_rect.top, m_rect.right, m_rect.bottom, m_level);*/
			m_subNodes[0]->Retrieve(container, rect);
		}
		if (m_subNodes[1]->ContainRect(rect))
		{
			/*DebugOut(L"Rect 1 %f %f %f %f level %d\n", m_rect.left, m_rect.top, m_rect.right, m_rect.bottom, m_level);*/
			m_subNodes[1]->Retrieve(container, rect);
		}
		if (m_subNodes[2]->ContainRect(rect))
		{
			/*DebugOut(L"Rect 2 %f %f %f %f level %d\n", m_rect.left, m_rect.top, m_rect.right, m_rect.bottom, m_level);*/
			m_subNodes[2]->Retrieve(container, rect);
		}
		if (m_subNodes[3]->ContainRect(rect))
		{
			/*DebugOut(L"Rect 3 %f %f %f %f level %d\n", m_rect.left, m_rect.top, m_rect.right, m_rect.bottom, m_level);*/
			m_subNodes[3]->Retrieve(container, rect);
		}

		return;
	}

	// Add all game objects to container
	for (const auto& obj : m_inNodes)
	{
		container.emplace_back(obj);
	}
}

void CQuadtree::Reset(float screen_width, float screen_height)
{
	m_rect = RectF(0, 0, screen_width, screen_height); // TODO: Instead of using static variable 
	m_inNodes.clear();
	m_inNodes.shrink_to_fit();

	m_subNodes[0] = nullptr;
	m_subNodes[1] = nullptr;
	m_subNodes[2] = nullptr;
	m_subNodes[3] = nullptr;
}

bool CQuadtree::Contain(CGameObject* gameObject)
{ 
	Vector2 posObj = gameObject->GetPosition();
	return m_rect.Contain(posObj); 
}

bool CQuadtree::ContainRect(const RectF& rect) const 
{
	return m_rect.Contain(rect);
}