#include "InputLayout.h"
#include "ExceptionMacros.h"
#include "Throwables.h"

Haku::InputLayout::InputLayout(ID3D11Device* Device, ID3DBlob* VertexBlobPointer, size_t VertexBlobSize)
{
	D3D11_INPUT_ELEMENT_DESC VertexInputDesc[]{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	EXCEPT_HR_THROW(Device->CreateInputLayout(
		VertexInputDesc, std::size(VertexInputDesc), VertexBlobPointer, VertexBlobSize, _InputLayout.GetAddressOf()))
}

void Haku::InputLayout::Bind(ID3D11DeviceContext* DeviceContext) noexcept
{
	DeviceContext->IASetInputLayout(_InputLayout.Get());
}
