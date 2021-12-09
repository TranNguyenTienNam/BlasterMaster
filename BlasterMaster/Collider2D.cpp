#include <algorithm>    
#include "Collider2D.h"
#include "Utils.h"
#include "Sophia.h"
#include "Jason.h"
#include "Playable.h"
#include "Enemy.h"
#include "BallCarry.h"
#include "Brick.h"

void CCollider2D::SweptAABB(
	RectF movingRect, RectF staticRect,
	float dx, float dy,
	float& nx, float& ny, float& t)
{
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry, t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0.0f;

	//
	// Broad-phase test 
	//

	RectF rBroadPhase;
	rBroadPhase.left = dx > 0 ? movingRect.left : movingRect.left + dx;
	rBroadPhase.top = dy > 0 ? movingRect.top + dy : movingRect.top;
	rBroadPhase.right = dx > 0 ? movingRect.right + dx : movingRect.right;
	rBroadPhase.bottom = dy > 0 ? movingRect.bottom : movingRect.bottom + dy;

	if (!rBroadPhase.Overlap(staticRect)) return;

	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = staticRect.left - movingRect.right;
		dx_exit = staticRect.right - movingRect.left;
	}
	else if (dx < 0)
	{
		dx_entry = staticRect.right - movingRect.left;
		dx_exit = staticRect.left - movingRect.right;
	}

	if (dy > 0)
	{
		dy_entry = staticRect.bottom - movingRect.top;
		dy_exit = staticRect.top - movingRect.bottom;
	}
	else if (dy < 0)
	{
		dy_entry = staticRect.top - movingRect.bottom;
		dy_exit = staticRect.bottom - movingRect.top;
	}

	if (dx == 0)
	{
		tx_entry = -VectorInfinity().x;
		tx_exit = VectorInfinity().x;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -VectorInfinity().y;
		ty_exit = VectorInfinity().y;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}

	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CCollider2D::SweptAABBEx(CCollider2D* coOther)
{
	RectF staticRect;		// static object bbox
	RectF movingRect;		// moving object bbox
	float t, nx, ny;

	float dt = CGame::GetDeltaTime();

	staticRect = coOther->GetBoundingBox();

	// deal with moving object: m speed = original m speed - collide object speed
	auto coObject = coOther->GetGameObject();
	float sdx = coObject->GetVelocity().x * dt;
	float sdy = coObject->GetVelocity().y * dt;

	auto velSelf = object->GetVelocity();
	this->dx = velSelf.x * dt;
	this->dy = velSelf.y * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = dx - sdx;
	float rdy = dy - sdy;

	movingRect = GetBoundingBox();

	SweptAABB(
		movingRect, staticRect,
		rdx, rdy, nx, ny, t);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, coObject, coOther);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CCollider2D::CalcPotentialCollisions(
	std::vector<CGameObject*>* coObjects,
	std::vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (object == coObjects->at(i)) continue;
		if (coObjects->at(i)->IsEnabled() == false) continue;

		// TODO: Filter by object tag
		auto selfTag = object->GetTag();
		auto otherTag = coObjects->at(i)->GetTag();
		if ((selfTag == ObjectTag::InvinciblePlayer && otherTag == ObjectTag::Enemy) ||
			(otherTag == ObjectTag::InvinciblePlayer && selfTag == ObjectTag::Enemy)) 
			continue;

		for (auto co : coObjects->at(i)->GetColliders())
		{
			LPCOLLISIONEVENT e = SweptAABBEx(co);

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CCollider2D::FilterCollision(
	std::vector<LPCOLLISIONEVENT>& coEvents,
	LPCOLLISIONEVENT& colX,
	LPCOLLISIONEVENT& colY,
	bool filterX = true,
	bool filterY = true)
{
	float min_tx, min_ty;

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->isDeleted == true) continue;
		if (c->co->IsTrigger() == true) continue;

		if (c->t < min_tx && c->nx != 0 && filterX == true) {
			min_tx = c->t; min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0 && filterY == true) {
			min_ty = c->t; min_iy = i;
		}
	}

	if (min_ix >= 0) colX = coEvents[min_ix];
	if (min_iy >= 0) colY = coEvents[min_iy];
}

void CCollider2D::PhysicsUpdate(std::vector<CGameObject*>* coObjects)
{
	// Reduce the number of check collision if game object is not enabled, is nullptr, static
	if (object == nullptr || isDynamic == false) return;

	DealWithOverlappedCase(coObjects);

	auto dt = CGame::GetDeltaTime();
	auto pos = object->GetPosition();
	auto velocity = object->GetVelocity();

	this->dx = velocity.x * dt;
	this->dy = velocity.y * dt;

	coEvents.clear();
	coEventX = NULL;
	coEventY = NULL;

	CalcPotentialCollisions(coObjects, coEvents); // TODO: Is it necessary to get a vector contains trigger objects?

	if (coEvents.size() == 0)
	{
		/*if (dynamic_cast<CSophia*>(object)) DebugOut(L"size 0\n");*/

		pos.x += dx;
		pos.y += dy;
	}
	else
	{
		FilterCollision(coEvents, coEventX, coEventY);

		if (coEventX != NULL && coEventY != NULL)
		{
			// was collision on Y first ?
			if (coEventY->t < coEventX->t)
			{
				/*if (dynamic_cast<CSophia*>(object)) DebugOut(L"Y first\n");*/

				if (isTrigger == false)
				{
					pos.y += coEventY->t * dy + coEventY->ny * BLOCK_PUSH_FACTOR;

					velocity.y = 0;
					object->SetVelocity(velocity);
				}
				else
				{
					pos.y += dy;
				}

				object->SetPosition(pos);

				if (coEventY->ny != 0)
					if (isTrigger == false) object->OnCollisionEnter(this, coEventY);
					else object->OnTriggerEnter(this, coEventY);

				//
				// see if after correction on Y, is there still a collision on X ? 
				//
				LPCOLLISIONEVENT colX_other = NULL;

				//
				// check again if there is true collision on X 
				//

				// remove current collision event on X
				coEventX->isDeleted = true;

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABBEx(coEventX->co));

				// re-filter on X only
				FilterCollision(coEvents, colX_other, coEventY, /*filterX=*/1, /*filterY=*/0);

				if (colX_other != NULL)
				{
					if (isTrigger == false)
						pos.x += colX_other->t * dx + colX_other->nx * BLOCK_PUSH_FACTOR;
					else pos.x += coEventY->t * dx;

					velocity.x = 0;
					object->SetVelocity(velocity);

					if (isTrigger == false) object->OnCollisionEnter(this, colX_other);
					else object->OnTriggerEnter(this, colX_other);
				}
				else
				{
					pos.x += coEventY->t * dx;
				}
			}
			// collision on X first
			else
			{
				/*if (dynamic_cast<CSophia*>(object)) DebugOut(L"X first\n");*/

				if (isTrigger == false)
				{
					pos.x += coEventX->t * dx + coEventX->nx * BLOCK_PUSH_FACTOR;

					velocity.x = 0;
					object->SetVelocity(velocity);
				}
				else
				{
					pos.x += dx;
				}

				object->SetPosition(pos);

				if (coEventX->nx != 0)
					if (isTrigger == false) object->OnCollisionEnter(this, coEventX);
					else object->OnTriggerEnter(this, coEventX);

				//
				// see if after correction on X, is there still a collision on Y ? 
				//
				LPCOLLISIONEVENT colY_other = NULL;

				//
				// check again if there is true collision on X 
				//

				// remove current collision event on Y
				coEventY->isDeleted = true;

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABBEx(coEventY->co));

				// re-filter on Y only
				FilterCollision(coEvents, coEventX, colY_other, /*filterX=*/0, /*filterY=*/1);

				if (colY_other != NULL)
				{
					if (isTrigger == false)
						pos.y += colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;
					else pos.y += coEventX->t * dy;

					velocity.y = 0;
					object->SetVelocity(velocity);

					if (isTrigger == false) object->OnCollisionEnter(this, colY_other);
					else object->OnTriggerEnter(this, colY_other);
				}
				else
				{
					pos.y += coEventX->t * dy;
				}
			}
		}
		else
		{
			if (coEventX != NULL)
			{
				//if (dynamic_cast<CSophia*>(object)) DebugOut(L"X only\n");

				if (isTrigger == false)
				{
					pos.x += coEventX->t * dx + coEventX->nx * BLOCK_PUSH_FACTOR;
					pos.y += coEventX->t * dy;

					velocity.x = 0;
					object->SetVelocity(velocity);
				}
				else
				{
					pos.x += dx;
					pos.y += dy;
				}

				if (isTrigger == false) object->OnCollisionEnter(this, coEventX);
				else object->OnTriggerEnter(this, coEventX);
			}
			else
			{
				if (coEventY != NULL)
				{
					//if (dynamic_cast<CSophia*>(object)) DebugOut(L"Y only\n");

					if (isTrigger == false)
					{
						pos.x += coEventY->t * dx;
						pos.y += coEventY->t * dy + coEventY->ny * BLOCK_PUSH_FACTOR;

						velocity.y = 0;
						object->SetVelocity(velocity);
					}
					else
					{
						pos.x += dx;
						pos.y += dy;
					}

					if (isTrigger == false) object->OnCollisionEnter(this, coEventY);
					else object->OnTriggerEnter(this, coEventY);
				}
				// both colX & colY are NULL 
				else
				{
					//if (dynamic_cast<CSophia*>(object)) DebugOut(L"both null\n");
					pos.x += dx;
					pos.y += dy;
				}
			}
		}
	}

	object->SetPosition(pos);

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		if (coEvents[i]->co->IsTrigger() == true)
		{
			if (isTrigger == false) object->OnCollisionEnter(this, coEvents[i]);
			else object->OnTriggerEnter(this, coEvents[i]);
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

//void CCollider2D::PhysicsUpdate(std::vector<CGameObject*>* coObjects)
//{
//	// Reduce the number of check collision if game object is not enabled, is nullptr, static
//	if (object == nullptr || isDynamic == false) return;
//
//	DealWithOverlappedCase(coObjects);
//
//	auto dt = CGame::GetDeltaTime();
//	auto pos = object->GetPosition();
//	auto velocity = object->GetVelocity();
//
//	this->dx = velocity.x * dt;
//	this->dy = velocity.y * dt;
//
//	coEvents.clear();
//	coEventX = NULL;
//	coEventY = NULL;
//
//	if (dynamic_cast<CSophia*>(object)) DebugOut(L"v %f %f\n", velocity.x, velocity.y);
//
//	CalcPotentialCollisions(coObjects, coEvents); // TODO: Is it necessary to get a vector contains trigger objects?
//
//	if (coEvents.size() == 0)
//	{
//		/*if (dynamic_cast<CSophia*>(object)) DebugOut(L"size 0\n");*/
//
//		pos.x += dx;
//		pos.y += dy;
//	}
//	else
//	{
//		FilterCollision(coEvents, coEventX, coEventY);
//
//		if (coEventX != NULL && coEventY != NULL)
//		{
//			// was collision on Y first ?
//			if (coEventY->t < coEventX->t)
//			{
//				/*if (dynamic_cast<CSophia*>(object)) DebugOut(L"Y first\n");*/
//
//				PushingHandling(coEventY, 1);
//
//				if (coEventY->ny != 0)
//					if (isTrigger == false) object->OnCollisionEnter(this, coEventY);
//					else object->OnTriggerEnter(this, coEventY);
//
//				//
//				// see if after correction on Y, is there still a collision on X ? 
//				//
//				LPCOLLISIONEVENT colX_other = NULL;
//
//				//
//				// check again if there is true collision on X 
//				//
//
//				// remove current collision event on X
//				coEventX->isDeleted = true;
//
//				// replace with a new collision event using corrected location 
//				coEvents.push_back(SweptAABBEx(coEventX->co));
//
//				// re-filter on X only
//				FilterCollision(coEvents, colX_other, coEventY, /*filterX=*/1, /*filterY=*/0);
//
//				if (colX_other != NULL)
//				{
//					PushingHandling(colX_other, coEventY->t);
//
//					if (isTrigger == false) object->OnCollisionEnter(this, colX_other);
//					else object->OnTriggerEnter(this, colX_other);
//				}
//				else
//				{
//					pos.x += coEventY->t * dx;
//				}
//			}
//			// collision on X first
//			else
//			{
//				/*if (dynamic_cast<CSophia*>(object)) DebugOut(L"X first\n");*/
//
//				PushingHandling(coEventX, 1);
//
//				if (coEventX->nx != 0)
//					if (isTrigger == false) object->OnCollisionEnter(this, coEventX);
//					else object->OnTriggerEnter(this, coEventX);
//
//				//
//				// see if after correction on X, is there still a collision on Y ? 
//				//
//				LPCOLLISIONEVENT colY_other = NULL;
//
//				//
//				// check again if there is true collision on X 
//				//
//
//				// remove current collision event on Y
//				coEventY->isDeleted = true;
//
//				// replace with a new collision event using corrected location 
//				coEvents.push_back(SweptAABBEx(coEventY->co));
//
//				// re-filter on Y only
//				FilterCollision(coEvents, coEventX, colY_other, /*filterX=*/0, /*filterY=*/1);
//
//				if (colY_other != NULL)
//				{
//					PushingHandling(colY_other, coEventX->t);
//
//					if (isTrigger == false) object->OnCollisionEnter(this, colY_other);
//					else object->OnTriggerEnter(this, colY_other);
//				}
//				else
//				{
//					pos.y += coEventX->t * dy;
//				}
//			}
//		}
//		else
//		{
//			if (coEventX != NULL)
//			{
//				//if (dynamic_cast<CSophia*>(object)) DebugOut(L"X only\n");
//
//				if (isTrigger == false)
//				{
//					pos.x += coEventX->t * dx + coEventX->nx * BLOCK_PUSH_FACTOR;
//					pos.y += coEventX->t * dy;
//
//					velocity.x = 0;
//					object->SetVelocity(velocity);
//				}
//				else
//				{
//					pos.x += dx;
//					pos.y += dy;
//				}
//
//				if (isTrigger == false) object->OnCollisionEnter(this, coEventX);
//				else object->OnTriggerEnter(this, coEventX);
//			}
//			else
//			{
//				if (coEventY != NULL)
//				{
//					//if (dynamic_cast<CSophia*>(object)) DebugOut(L"Y only\n");
//
//					if (isTrigger == false)
//					{
//						pos.x += coEventY->t * dx;
//						pos.y += coEventY->t * dy + coEventY->ny * BLOCK_PUSH_FACTOR;
//
//						velocity.y = 0;
//						object->SetVelocity(velocity);
//					}
//					else
//					{
//						pos.x += dx;
//						pos.y += dy;
//					}
//
//					if (isTrigger == false) object->OnCollisionEnter(this, coEventY);
//					else object->OnTriggerEnter(this, coEventY);
//				}
//				// both colX & colY are NULL 
//				else
//				{
//					//if (dynamic_cast<CSophia*>(object)) DebugOut(L"both null\n");
//					pos.x += dx;
//					pos.y += dy;
//				}
//			}
//		}
//	}
//
//	object->SetPosition(pos);
//
//	for (UINT i = 0; i < coEvents.size(); i++)
//	{
//		if (coEvents[i]->co->IsTrigger() == true)
//		{
//			if (isTrigger == false) object->OnCollisionEnter(this, coEvents[i]);
//			else object->OnTriggerEnter(this, coEvents[i]);
//		}
//	}
//
//	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
//}

void CCollider2D::PushingHandling(LPCOLLISIONEVENT& coEvent, float previousCollisionDuration = 1.0f)
{
	auto pos = object->GetPosition();
	auto velocity = object->GetVelocity();

	if (isTrigger == false)
	{
		if (coEvent->nx != 0)
		{
			pos.x += coEvent->t * dx + coEvent->nx * BLOCK_PUSH_FACTOR;
			velocity.x = 0;
		}

		if (coEvent->ny != 0)
		{
			pos.y += coEvent->t * dy + coEvent->ny * BLOCK_PUSH_FACTOR;
			velocity.y = 0;
		}

		object->SetVelocity(velocity);
		if (dynamic_cast<CSophia*>(object)) DebugOut(L"v %f %f\n", velocity.x, velocity.y);
	}
	else
	{
		if (coEvent->nx != 0) pos.x += dx * previousCollisionDuration;
		if (coEvent->ny != 0) pos.y += dy * previousCollisionDuration;
	}

	object->SetPosition(pos);
}

void CCollider2D::DealWithOverlappedCase(std::vector<CGameObject*>* coObjects)
{
	if (isTrigger == true) return;

	std::vector<CGameObject*> overlappedObjects;

	for (auto coO : *coObjects)
	{
		if (coO == object) continue;
		if (coO->GetColliders().size() == 0) continue;

		auto selfTag = object->GetTag();
		auto otherTag = coO->GetTag();

		auto coOther = coO->GetColliders().at(0);
		if (coOther->isTrigger == true) continue;

		auto bbOther = coOther->GetBoundingBox();
		auto bbSelf = GetBoundingBox();

		if (bbSelf.Overlap(bbOther) && dynamic_cast<CBrick*>(coO))
			overlappedObjects.emplace_back(coO);
	}

	for (auto coO : overlappedObjects)
	{
		auto bbOther = coO->GetColliders().at(0)->GetBoundingBox();
		auto bbSelf = GetBoundingBox();

		if (bbSelf.Overlap(bbOther) == false) continue;

		float deltaX = 0, deltaY = 0;

		if (bbSelf.left < bbOther.right && bbSelf.left > bbOther.left)
		{
			deltaX += bbOther.right - bbSelf.left + BLOCK_PUSH_FACTOR;
		}

		if (bbSelf.right > bbOther.left && bbSelf.right < bbOther.right)
		{
			deltaX += -1 * (bbSelf.right - bbOther.left + BLOCK_PUSH_FACTOR);
		}

		if (bbSelf.bottom < bbOther.top && bbSelf.bottom > bbOther.bottom)
		{
			deltaY += bbOther.top - bbSelf.bottom + BLOCK_PUSH_FACTOR;
		}

		if (bbSelf.top > bbOther.bottom && bbSelf.top < bbOther.top)
		{
			deltaY += -1 * (bbSelf.top - bbOther.bottom + BLOCK_PUSH_FACTOR);
		}

		if (deltaX != 0 && deltaY != 0)
		{
			if (abs(deltaX) <= abs(deltaY))
			{
				deltaY = 0;
			}
			else deltaX = 0;
		}

		auto newPos = object->GetPosition();
		newPos += Vector2(deltaX, deltaY);
		object->SetPosition(newPos);
	}
}

RectF CCollider2D::GetBoundingBox()
{
	Vector2 posCollider = object->GetPosition() + offset;
	RectF boundingBox;
	boundingBox.left = posCollider.x - boxSize.x / 2;
	boundingBox.top = posCollider.y + boxSize.y / 2;
	boundingBox.right = posCollider.x + boxSize.x / 2;
	boundingBox.bottom = posCollider.y - boxSize.y / 2;
	return boundingBox;
}

void CCollider2D::RenderBoundingBox()
{
	Vector2 posCollider = object->GetPosition() + offset;

	LPDIRECT3DTEXTURE9 bbox = CGame::GetInstance()->GetService<CTextures>()->Get("tex-bbox");

	RectF rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = boxSize.x;
	rect.bottom = boxSize.y;

	CGame::GetInstance()->Draw(posCollider, -1, bbox, rect.left, rect.top, rect.right, rect.bottom, D3DCOLOR_ARGB(100, 255, 255, 255));
}
