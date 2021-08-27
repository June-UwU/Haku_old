#pragma once
#include "GPUDataType.h"
#include "Buffer.h"

#include "wil/wrl.h"
#include <filesystem>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>

class Graphics;

class Model
{
public:
	Model(
		ID3D11Device*		 Device,
		ID3D11DeviceContext* DeviceContext,
		std::vector<int>&&	 Index,
		std::vector<Point>&& Vertex);
	void Bind(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);

private:
	std::unique_ptr<Haku::VertexBuffer>		 VertexData;
	std::unique_ptr<Haku::IndexBuffer>		 IndexData;
	std::unique_ptr<Haku::VertexConstBuffer> ConstBufferVertex;

	ConstVertexData							   Matrix;
	float									   ClientWidth;
	float									   ClientHeight;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	   RotationMatrix;
	Microsoft::WRL::ComPtr<ID3DBlob>		   ErrorBlob;
	Microsoft::WRL::ComPtr<ID3DBlob>		   VertexBlob;
	Microsoft::WRL::ComPtr<ID3DBlob>		   PixelBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  PixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  InputLayout;
};
