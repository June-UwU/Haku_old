#pragma once
#include "Graphics.h"
struct Point
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Col;
};
struct Rotation
{
	DirectX::XMMATRIX RotationZ;
};

class AssetManager
{
public:
	AssetManager();
	AssetManager(Graphics* GFX);
	void SetGraphics(Graphics* Pointer) noexcept;
	void ReadModel(std::string& path, float ThetaZ, float translation);
	void Draw(float ThetaZ, float translation) noexcept;
	void Test(float ThetaZ, float translation);
private:
	Graphics* GFX = nullptr;

	/*TEST*/
	std::vector<int>						   Indexdata;
	std::vector<Point>						   VertexData;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	   RotationMatrix;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	   VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	   IndexBuffer;
	Microsoft::WRL::ComPtr<ID3DBlob>		   ErrorBlob;
	Microsoft::WRL::ComPtr<ID3DBlob>		   VertexBlob;
	Microsoft::WRL::ComPtr<ID3DBlob>		   PixelBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  PixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  InputLayout;
};
