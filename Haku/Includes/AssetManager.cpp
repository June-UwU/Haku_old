#include "AssetManager.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "GPUDataType.h"

AssetManager::AssetManager()
	: GFX(nullptr)
	, TestData{}
{
}

void AssetManager::AddLights()
{
	LightPointer = std::make_unique<Haku::Lights>(LightModelVertexBuffer, LightModelIndexBuffer, GFX->_Device.Get());
}

AssetManager::AssetManager(Graphics* GFX)
	: GFX(GFX)
{
}

void AssetManager::SetGraphics(Graphics* Pointer) noexcept
{
	GFX = Pointer;

	char FilePath[256];
	GetModuleFileNameA(nullptr, FilePath, std::size(FilePath));
	std::filesystem::path Exe(FilePath);
	Exe.remove_filename();
	std::filesystem::path ModelPath(Exe / "../../Model/lightmodel.obj");
	std::string			  path = ModelPath.string();

	const auto		 DeviceContext = GFX->_DeviceContext.Get();
	const auto		 Device		   = GFX->_Device.Get();
	Assimp::Importer Importer;

	const aiScene* Scene = Importer.ReadFile(
		path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	const auto		   meshes = *Scene->mMeshes;
	std::vector<Point> VertexData;
	VertexData.reserve(meshes->mNumVertices);
	for (int i = 0; i < meshes->mNumVertices; i++)
	{
		VertexData.push_back({ { meshes->mVertices[i].x, meshes->mVertices[i].y, meshes->mVertices[i].z } });
	}

	std::vector<int> Indexdata;
	Indexdata.reserve(static_cast<int64_t>(meshes->mNumFaces) * 3);
	for (int i = 0; i < meshes->mNumFaces; i++)
	{
		auto& Face = meshes->mFaces[i];
		Indexdata.push_back(Face.mIndices[0]);
		Indexdata.push_back(Face.mIndices[1]);
		Indexdata.push_back(Face.mIndices[2]);
	}

	LightModelIndexBuffer  = std::make_shared<Haku::IndexBuffer>(std::move(Indexdata), Device);
	LightModelVertexBuffer = std::make_shared<Haku::VertexBuffer<Point>>(std::move(VertexData), Device);
}

void AssetManager::ReadModel(const std::string& path)
{
	const auto		 DeviceContext = GFX->_DeviceContext.Get();
	const auto		 Device		   = GFX->_Device.Get();
	Assimp::Importer Importer;

	const aiScene* Scene = Importer.ReadFile(
		path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	const auto		   meshes = *Scene->mMeshes;
	std::vector<Point> VertexData;
	VertexData.reserve(meshes->mNumVertices);
	for (int i = 0; i < meshes->mNumVertices; i++)
	{
		VertexData.push_back({ { meshes->mVertices[i].x, meshes->mVertices[i].y, meshes->mVertices[i].z } });
	}

	std::vector<int> Indexdata;
	Indexdata.reserve(static_cast<int64_t>(meshes->mNumFaces) * 3);
	for (int i = 0; i < meshes->mNumFaces; i++)
	{
		auto& Face = meshes->mFaces[i];
		Indexdata.push_back(Face.mIndices[0]);
		Indexdata.push_back(Face.mIndices[1]);
		Indexdata.push_back(Face.mIndices[2]);
	}

	EntityVector.emplace_back(
		Model(Device, DeviceContext, std::move(Indexdata), std::move(VertexData), GFX->ClientWidth, GFX->ClientHeight));
}

void AssetManager::Draw() noexcept
{
	ID3D11Device*		 Device		   = GFX->_Device.Get();
	ID3D11DeviceContext* DeviceContext = GFX->_DeviceContext.Get();
	D3D11_VIEWPORT		 vp;
	vp.Width	= GFX->ClientWidth;
	vp.Height	= GFX->ClientHeight;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DeviceContext->RSSetViewports(1u, &vp);
	if (LightPointer)
	{
		LightPointer->Bind(GFX->_DeviceContext.Get());
		DeviceContext->DrawIndexed(LightModelIndexBuffer->GetIndicesNo(), 0u, 0u);
		LightPointer->BindConstantBuffer(DeviceContext);
	}
	for (Model& m : EntityVector)
	{
		m.XTranslation(TestData.XTrans);
		m.YTranslation(TestData.YTrans);
		m.ZTranslation(TestData.ZTrans);
		m.XRotate(TestData.XRotate);
		m.YRotate(TestData.YRotate);
		m.ZRotate(TestData.ZRotate);

		m.Bind(Device, DeviceContext);
		DeviceContext->DrawIndexed(m.GetIndexSize(), 0u, 0u);
	}
}

void AssetManager::UpdateTestData(ConstVertexModifer& Ref) noexcept
{
	TestData.XTrans	 = Ref.XTrans;
	TestData.YTrans	 = Ref.YTrans;
	TestData.ZTrans	 = Ref.ZTrans;
	TestData.XPos	 = Ref.XPos;
	TestData.YPos	 = Ref.YPos;
	TestData.ZPos	 = Ref.ZPos;
	TestData.XRotate = Ref.XRotate;
	TestData.YRotate = Ref.YRotate;
	TestData.ZRotate = Ref.ZRotate;
}
