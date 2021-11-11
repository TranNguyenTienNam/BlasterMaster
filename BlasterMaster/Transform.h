#pragma once
//#include <d3d9.h>
#include <d3dx9.h>

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;

class Transform
{
public:
	Vector2 position;
	Vector2 rotation;
	float scale;
};

Vector2 VectorInfinity();
Vector2 VectorZero();
Vector2 NormalizedVector();