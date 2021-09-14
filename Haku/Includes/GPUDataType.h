#pragma once
#include <DirectXMath.h>

struct ConstVertexModifer
{
	float XTrans;
	float YTrans;
	float ZTrans;
	float XPos;
	float YPos;
	float ZPos;
	float XRotate;
	float YRotate;
	float ZRotate;
};

struct LightingConst
{
	DirectX::XMFLOAT3 AmbientSource;
	DirectX::XMFLOAT3 Diffuse;
	DirectX::XMFLOAT3 Position;
	float AttenConst;
	float AttenLinear;
	float AttenQuad;
};


struct Point
{
	DirectX::XMFLOAT3 Pos;
};
struct ConstVertexData
{
	DirectX::XMMATRIX RotationZ;
};