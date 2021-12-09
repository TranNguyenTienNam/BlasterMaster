#include "Camera.h"
#include "Game.h"
#include "Utils.h"

CCamera::CCamera()
{
	position = VectorInfinity();
}

CCamera::~CCamera()
{
}

Vector2 CCamera::WorldToScreenPoint(Vector2 pos)
{
	Vector3 p = Vector3(0, 0, 0);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	// Translate
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, (pos.x - position.x), (-pos.y + position.y), 0.0f);

	mat *= translate;

	return Vector2((int)mat._41, (int)mat._42);
}

RectF CCamera::GetBoundingBox()
{
	Vector2 nextPos = position + target->GetVelocity() * CGame::GetDeltaTime();

	RectF boundingBox;
	boundingBox.left = min(position.x, nextPos.x);
	boundingBox.top = max(position.y, nextPos.y);
	boundingBox.right = max(position.x + bbSize.x, nextPos.x + bbSize.x);
	boundingBox.bottom = min(position.y - bbSize.y, nextPos.y - bbSize.y);

	return boundingBox;
}

void CCamera::Update()
{
	Vector2 posTarget = target->GetPosition();

	Vector2 vpPlayer = WorldToScreenPoint(posTarget);
	
	if (position == VectorInfinity())
	{
		position.x = (int)(posTarget.x - bbSize.x / 2);
		position.y = (int)(posTarget.y + bbSize.y / 2);
	}
	else
	{
		// freeze camera
		if (vpPlayer.x <= freezeBoundary.left)
			position.x = (int)(posTarget.x - freezeBoundary.left);
		if (vpPlayer.x >= freezeBoundary.right)
			position.x = (int)(posTarget.x - freezeBoundary.right);
		if (vpPlayer.y <= freezeBoundary.top)
			position.y = (int)(posTarget.y + freezeBoundary.top);
		if (vpPlayer.y >= freezeBoundary.bottom)
			position.y = (int)(posTarget.y + freezeBoundary.bottom);
	}

	// Boundary block
	if (position.x <= boundary.left)
		position.x = boundary.left;
	if (position.y >= boundary.top)
		position.y = boundary.top;
	if (position.x + bbSize.x >= boundary.right)
		position.x = boundary.right - bbSize.x;
	if (position.y - bbSize.y <= boundary.bottom)
		position.y = boundary.bottom + bbSize.y;
}