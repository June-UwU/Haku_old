#include "AssetManager.h"

/*COMPRESSION INCLUDES*/

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "ExceptionMacros.h"
#include "Throwables.h"
#include "wil/wrl.h"
#include <filesystem>
#include <d3d11.h>
#include <d3dcompiler.h>

/*--------------------*/

#include <DirectXMath.h>
#include <vector>

AssetManager::AssetManager()
	: GFX(nullptr)
{
}

AssetManager::AssetManager(Graphics* GFX)
	: GFX(GFX)
{
}

void AssetManager::SetGraphics(Graphics* Pointer) noexcept
{
	GFX = Pointer;
}

void AssetManager::ReadModel(std::string& path, float ThetaZ, float translation)
{
	auto			 DeviceContext = GFX->_DeviceContext.Get();
	auto			 Device		   = GFX->_Device.Get();
	Assimp::Importer Importer;

	const aiScene* Scene = Importer.ReadFile(
		path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	auto meshes = *Scene->mMeshes;
	VertexData.reserve(meshes->mNumVertices);
	for (int i = 0; i < meshes->mNumVertices; i++)
	{
		VertexData.push_back({ { meshes->mVertices[i].x, meshes->mVertices[i].y, meshes->mVertices[i].z } });
	}

	Indexdata.reserve(int64_t(meshes->mNumFaces) * 3);
	for (int i = 0; i < meshes->mNumFaces; i++)
	{
		auto& Face = meshes->mFaces[i];
		Indexdata.push_back(Face.mIndices[0]);
		Indexdata.push_back(Face.mIndices[1]);
		Indexdata.push_back(Face.mIndices[2]);
	}
	// run it
	char FilePath[256];
	GetModuleFileNameA(nullptr, FilePath, std::size(FilePath));
	std::filesystem::path Exe(FilePath);
	Exe.remove_filename();
	std::filesystem::path VertexShaderPath(Exe / "../../Shaders/VertexShader1.hlsl");
	std::filesystem::path PixelShaderPath(Exe / "../../Shaders/PixelShader1.hlsl");

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	// Men, shader isn't part of the model
	EXCEPT_HR_THROW(D3DCompileFromFile(
		VertexShaderPath.wstring().c_str(),
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		flags,
		NULL,
		VertexBlob.GetAddressOf(),
		ErrorBlob.GetAddressOf()))

	EXCEPT_HR_THROW(D3DCompileFromFile(
		PixelShaderPath.wstring().c_str(),
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		flags,
		NULL,
		PixelBlob.GetAddressOf(),
		ErrorBlob.GetAddressOf()))

	EXCEPT_HR_THROW(Device->CreateVertexShader(
		VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), nullptr, VertexShader.GetAddressOf()))
	EXCEPT_HR_THROW(Device->CreatePixelShader(
		PixelBlob->GetBufferPointer(), PixelBlob->GetBufferSize(), nullptr, PixelShader.GetAddressOf()))

	D3D11_BUFFER_DESC VertexDesc{};
	VertexDesc.ByteWidth		   = VertexData.size() * sizeof(Point);
	VertexDesc.Usage			   = D3D11_USAGE_DEFAULT;
	VertexDesc.BindFlags		   = D3D11_BIND_VERTEX_BUFFER;
	VertexDesc.CPUAccessFlags	   = 0;
	VertexDesc.MiscFlags		   = 0;
	VertexDesc.StructureByteStride = sizeof(Point);

	D3D11_SUBRESOURCE_DATA VertexSubRes{};
	VertexSubRes.pSysMem = VertexData.data();

	D3D11_BUFFER_DESC IndexBufferDesc{};
	IndexBufferDesc.ByteWidth	   = sizeof(int) * Indexdata.size();
	IndexBufferDesc.Usage		   = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.BindFlags	   = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags	   = 0;

	D3D11_SUBRESOURCE_DATA IndexSubRes{ 0 };
	IndexSubRes.pSysMem = Indexdata.data();

	EXCEPT_HR_THROW(Device->CreateBuffer(&IndexBufferDesc, &IndexSubRes, IndexBuffer.GetAddressOf()))
	EXCEPT_HR_THROW(Device->CreateBuffer(&VertexDesc, &VertexSubRes, VertexBuffer.GetAddressOf()))

	D3D11_INPUT_ELEMENT_DESC VertexInputDesc[]{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	/*CONSTANT BUFFER*/

	Rotation Matrix{

		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationX(ThetaZ) * DirectX::XMMatrixRotationZ(ThetaZ) *
			DirectX::XMMatrixRotationY(ThetaZ) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f) *
			DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) *
			DirectX::XMMatrixPerspectiveFovLH(90, GFX->ClientWidth / GFX->ClientHeight, 0.5f, 100.0f))
	};
	D3D11_BUFFER_DESC ConstantBuffer{};
	ConstantBuffer.ByteWidth	  = sizeof(Matrix);
	ConstantBuffer.Usage		  = D3D11_USAGE_DYNAMIC;
	ConstantBuffer.BindFlags	  = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA ConstantSubResource{};
	ConstantSubResource.pSysMem = &Matrix;

	EXCEPT_HR_THROW(Device->CreateBuffer(&ConstantBuffer, &ConstantSubResource, RotationMatrix.GetAddressOf()))
	/*---------------*/

	EXCEPT_HR_THROW(Device->CreateInputLayout(
		VertexInputDesc,
		std::size(VertexInputDesc),
		VertexBlob->GetBufferPointer(),
		VertexBlob->GetBufferSize(),
		InputLayout.GetAddressOf()))
}

void AssetManager::Draw(float ThetaZ, float translation) noexcept
{
	auto DeviceContext = GFX->_DeviceContext.Get();
	auto Device		   = GFX->_Device.Get();
	DeviceContext->VSSetShader(VertexShader.Get(), 0, 0);
	DeviceContext->PSSetShader(PixelShader.Get(), 0, 0);
	UINT	 stride = sizeof(Point);
	UINT	 OffSet = 0u;
	Rotation Matrix{

		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationX(ThetaZ) * DirectX::XMMatrixRotationZ(ThetaZ) *
			DirectX::XMMatrixRotationY(ThetaZ) * DirectX::XMMatrixTranslation(0.0f, 0.0f, translation + 4.0f) *
			DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) *
			DirectX::XMMatrixPerspectiveFovLH(90, GFX->ClientWidth / GFX->ClientHeight, 0.5f, 100.0f))
	}; // list initialization works...!!!or does it..!

	D3D11_BUFFER_DESC ConstantBuffer{};
	ConstantBuffer.ByteWidth	  = sizeof(Matrix);
	ConstantBuffer.Usage		  = D3D11_USAGE_DYNAMIC;
	ConstantBuffer.BindFlags	  = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA ConstantSubResource{};
	ConstantSubResource.pSysMem = &Matrix;

	EXCEPT_HR_THROW(Device->CreateBuffer(&ConstantBuffer, &ConstantSubResource, RotationMatrix.GetAddressOf()))
	DeviceContext->VSSetConstantBuffers(0u, 1u, RotationMatrix.GetAddressOf());
	DeviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &OffSet);
	DeviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	DeviceContext->IASetInputLayout(InputLayout.Get());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT vp;
	vp.Width	= GFX->ClientWidth;
	vp.Height	= GFX->ClientHeight;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DeviceContext->RSSetViewports(1u, &vp);
	DeviceContext->DrawIndexed(Indexdata.size(), 0, 0);
}
