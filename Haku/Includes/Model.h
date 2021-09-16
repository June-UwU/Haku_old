#pragma once
#include "GPUDataType.h"
#include "Buffer.h"

#include "wil/wrl.h"
#include <filesystem>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>

class Model
{
public:
	Model(
		ID3D11Device*		 Device,
		ID3D11DeviceContext* DeviceContext,
		std::vector<int>&&	 Index,
		std::vector<Point>&& Vertex,
		float				 ClientWidth,
		float				 ClientHeight);
	void	  Bind(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);
	const int GetIndexSize() noexcept { return IndexData->GetIndicesNo(); }
	void	  XTranslation(float Value) noexcept;
	void	  YTranslation(float Value) noexcept;
	void	  ZTranslation(float Value) noexcept;
	void	  XRotate(float Value) noexcept;
	void	  YRotate(float Value) noexcept;
	void	  ZRotate(float Value) noexcept;

private:
	std::unique_ptr<Haku::VertexBuffer<Point>> VertexData;
	std::unique_ptr<Haku::IndexBuffer>		   IndexData;
	std::unique_ptr<Haku::VertexConstBuffer>   ConstBufferVertex;
	ConstVertexModifer						   ModelData;
};
