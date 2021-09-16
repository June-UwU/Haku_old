#include "Model.h"
#include <DirectXMath.h>

#include "ExceptionMacros.h"
#include "Throwables.h"

Model::Model(
	ID3D11Device*		 Device,
	ID3D11DeviceContext* DeviceContext,
	std::vector<int>&&	 Index,
	std::vector<Point>&& Vertex,
	float				 ClientWidth,
	float				 ClientHeight)
	: ModelData{}
{
	VertexData		  = std::make_unique<Haku::VertexBuffer<Point>>(std::move(Vertex), Device);
	IndexData		  = std::make_unique<Haku::IndexBuffer>(std::move(Index), Device);
	ConstBufferVertex = std::make_unique<Haku::VertexConstBuffer>(Device, ClientWidth, ClientHeight);
	char FilePath[256];
	GetModuleFileNameA(nullptr, FilePath, std::size(FilePath));
	std::filesystem::path Exe(FilePath);
	Exe.remove_filename();
	const std::filesystem::path VertexShaderPath(Exe / "../../Shaders/VertexShader1.hlsl");
	const std::filesystem::path PixelShaderPath(Exe / "../../Shaders/PixelShader1.hlsl");

}

void Model::Bind(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	VertexData->Bind(DeviceContext);
	IndexData->Bind(DeviceContext);
	ConstBufferVertex->UpdateParameters(DeviceContext, &ModelData);
	ConstBufferVertex->Bind(DeviceContext);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::XTranslation(float Value) noexcept
{
	ModelData.XTrans = Value;
}

void Model::YTranslation(float Value) noexcept
{
	ModelData.YTrans = Value;
}

void Model::ZTranslation(float Value) noexcept
{
	ModelData.ZTrans = Value;
}

void Model::XRotate(float Value) noexcept
{
	ModelData.XRotate = Value;
}

void Model::YRotate(float Value) noexcept
{
	ModelData.YRotate = Value;
}

void Model::ZRotate(float Value) noexcept
{
	ModelData.ZRotate = Value;
}
