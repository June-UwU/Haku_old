#include "Lighting.h"

Haku::Lights::Lights(
	std::shared_ptr<VertexBuffer<Point>> ModelPointer,
	std::shared_ptr<IndexBuffer>		 ModelIndexBuffer,
	ID3D11Device*						 Device)
{
	ResetLightingConsts();
	ConstBuffer		 = std::make_unique<PixelConstBuffer<LightingConst>>(Device, BufferValue);
	LightModelBuffer = std::shared_ptr<VertexBuffer<Point>>(ModelPointer);
	LightModelIndex	 = std::shared_ptr<IndexBuffer>(ModelIndexBuffer);
}

void Haku::Lights::Bind(ID3D11DeviceContext* DeviceContext) noexcept
{
	LightModelBuffer->Bind(DeviceContext);
}

void Haku::Lights::ResetLightingConsts() noexcept
{
	BufferValue = {
		{ 0.0f, 0.0f, 0.0f }, // Position
		{ 0.5f, 0.5f, 0.5f }, // Ambient Source
		{ 1.0f, 1.0f, 1.0f }, // Diffuse Source
		1.0f,				  // Diffuse Intensity
		1.0f,				  // Attenuation Constant
		0.045f,				  // Attenuation Linear Value
		0.0075f				  // Attenuation Quadratic Value
	};
}

const int Haku::Lights::GetIndexSize() noexcept
{
	return LightModelIndex->GetIndicesNo();
}

void Haku::Lights::BindConstantBuffer(ID3D11DeviceContext* DeviceContext) noexcept
{
	ConstBuffer->Bind(DeviceContext);
}
