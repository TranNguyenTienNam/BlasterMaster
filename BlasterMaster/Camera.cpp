#include "Camera.h"
#include "Game.h"
#include "Utils.h"

CCamera::CCamera()
{
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

	return Vector2(mat._41, mat._42);
}

RectF CCamera::GetBoundingBox()
{
	RectF boundingBox{ 0,0,0,0 };
	boundingBox.left = position.x;
	boundingBox.top = position.y;
	boundingBox.right = position.x + bbSize.x;
	boundingBox.bottom = position.y  - bbSize.y;
	return boundingBox;
}

void CCamera::Update()
{
	Vector2 posTarget = target->GetPosition();

	Vector2 vpPlayer = WorldToScreenPoint(posTarget);
	/*DebugOut(L"wp %f %f\n", posTarget.x, posTarget.y);*/
	/*DebugOut(L"vp %f %f\n", vpPlayer.x, vpPlayer.y);*/	
	
	position.x = posTarget.x - bbSize.x / 2;
	position.y = posTarget.y + bbSize.y / 2;

	// static camera
	/*if (vpPlayer.x >= 48 && vpPlayer.x <= 208)
		position.x = posTarget.x - bbSize.x / 2;
	if (vpPlayer.y >= 64 && vpPlayer.y <= 192)
		position.y = posTarget.y + bbSize.y / 2;*/

	// Camera co 2 kieu: static va follow target
	// Static la camera khong chuyen dong khi target di chuyen trong mot rect nhat dinh 
	// vpPlayer.x <= 48 || vpPlayer.x >= 208 (3 tile left right) (goi tat Viewport player)
	// vpPlayer.y <= 64 || vpPlayer.y >= 192 (4 tile bottom top)

	// Boundary block
	/*if (position.x <= boundary.left)
		position.x = boundary.left;
	if (position.y >= boundary.top)
		position.y = boundary.top;
	if (position.x + bbSize.x >= boundary.right)
		position.x = boundary.right - bbSize.x;
	if (position.y - bbSize.y <= boundary.bottom)
		position.y = boundary.bottom + bbSize.y;*/
}