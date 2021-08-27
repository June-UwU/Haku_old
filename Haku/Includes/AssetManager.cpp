#include "AssetManager.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "GPUDataType.h"

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

void AssetManager::ReadModel(const std::string& path, float ThetaZ, float translation)
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

	EntityVector.emplace_back(Model(Device, DeviceContext, std::move(Indexdata), std::move(VertexData)));
}

void AssetManager::Draw() noexcept
{
	for (Model& m : EntityVector)
	{
		m.Bind(GFX->_Device.Get(), GFX->_DeviceContext.Get());
	}
}
