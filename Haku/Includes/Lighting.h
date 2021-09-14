#pragma once
#include "GPUDataType.h"
#include "Buffer.h"

namespace Haku
{
class Lights
{
public:
	Lights(
		std::shared_ptr<VertexBuffer<Point>> ModelPointer,
		std::shared_ptr<IndexBuffer>		 ModelIndexBuffer,
		ID3D11Device*						 Device);
	void	  Bind(ID3D11DeviceContext* DeviceContext) noexcept;
	void	  ResetLightingConsts() noexcept;
	const int GetIndexSize() noexcept;
	void	  BindConstantBuffer(ID3D11DeviceContext* DeviceContext) noexcept;

private:
	LightingConst									 BufferValue{};
	std::shared_ptr<IndexBuffer>					 LightModelIndex;
	std::shared_ptr<VertexBuffer<Point>>			 LightModelBuffer;
	std::unique_ptr<PixelConstBuffer<LightingConst>> ConstBuffer;
};
} // namespace Haku
