#include "Buffer.h"
#include "ExceptionMacros.h"
#include "Throwables.h"

namespace Haku
{
VertexBuffer::VertexBuffer(std::vector<Point>&& VertexData, ID3D11Device* Device)
	: stride(sizeof(Point))
	, OffSet(0)
{
	D3D11_BUFFER_DESC VertexDesc{};
	VertexDesc.ByteWidth		   = VertexData.size() * sizeof(Point);
	VertexDesc.Usage			   = D3D11_USAGE_DEFAULT;
	VertexDesc.BindFlags		   = D3D11_BIND_VERTEX_BUFFER;
	VertexDesc.CPUAccessFlags	   = 0;
	VertexDesc.MiscFlags		   = 0;
	VertexDesc.StructureByteStride = sizeof(Point);

	D3D11_SUBRESOURCE_DATA VertexSubRes{};
	VertexSubRes.pSysMem = VertexData.data();

	EXCEPT_HR_THROW(Device->CreateBuffer(&VertexDesc, &VertexSubRes, DataBuffer.GetAddressOf()))
}

void VertexBuffer::Bind(ID3D11DeviceContext* DeviceContext)
{
	DeviceContext->IASetVertexBuffers(0, 1, DataBuffer.GetAddressOf(), &stride, &OffSet);
}

IndexBuffer::IndexBuffer(std::vector<int>&& IndexData, ID3D11Device* Device)
	: BufferSize(IndexData.size())
{
	D3D11_BUFFER_DESC IndexBufferDesc{};
	IndexBufferDesc.ByteWidth	   = sizeof(int) * IndexData.size();
	IndexBufferDesc.Usage		   = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.BindFlags	   = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags	   = 0;

	D3D11_SUBRESOURCE_DATA IndexSubRes{ 0 };
	IndexSubRes.pSysMem = IndexData.data();

	EXCEPT_HR_THROW(Device->CreateBuffer(&IndexBufferDesc, &IndexSubRes, DataBuffer.GetAddressOf()))
}

void IndexBuffer::Bind(ID3D11DeviceContext* DeviceContext)
{
	DeviceContext->IASetIndexBuffer(DataBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

const int IndexBuffer::GetIndicesNo() noexcept
{
	return BufferSize;
}
} // namespace Haku
