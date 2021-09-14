#include "Buffer.h"

namespace Haku
{
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

VertexConstBuffer::VertexConstBuffer(ID3D11Device* Device, const float ClientWidth, const float ClientHeight)
	: ClientHeight(ClientHeight)
	, ClientWidth(ClientWidth)
{
	ConstVertexData	  Matrix{ DirectX::XMMatrixTranspose(
		  DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f) * DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) *
		  DirectX::XMMatrixPerspectiveFovLH(90, ClientWidth / ClientHeight, 0.5f, 100.0f)) };
	D3D11_BUFFER_DESC ConstantBuffer{};
	ConstantBuffer.ByteWidth	  = sizeof(Matrix);
	ConstantBuffer.Usage		  = D3D11_USAGE_DYNAMIC;
	ConstantBuffer.BindFlags	  = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA ConstantSubResource{};
	ConstantSubResource.pSysMem = &Matrix;

	EXCEPT_HR_THROW(Device->CreateBuffer(&ConstantBuffer, &ConstantSubResource, DataBuffer.GetAddressOf()))
}

void VertexConstBuffer::Bind(ID3D11DeviceContext* DeviceContext)
{
	DeviceContext->VSSetConstantBuffers(0u, 1u, DataBuffer.GetAddressOf());
}

void VertexConstBuffer::UpdateParameters(ID3D11DeviceContext* DeviceContext, ConstVertexModifer* Reference) 
{
	D3D11_MAPPED_SUBRESOURCE SubResource{};
	memset(&SubResource,0,sizeof(SubResource));
	EXCEPT_HR_THROW(DeviceContext->Map(DataBuffer.Get(), 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource))
	DirectX::XMMATRIX Matrix = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationX(Reference->XRotate) * DirectX::XMMatrixRotationY(Reference->YRotate) *
		DirectX::XMMatrixRotationZ(Reference->ZRotate) *
		DirectX::XMMatrixTranslation(Reference->XTrans, Reference->YTrans, 4.0f + Reference->ZTrans) *
		DirectX::XMMatrixPerspectiveFovLH(90, ClientWidth / ClientHeight, 0.5f, 100.0f));
	memcpy(SubResource.pData, &Matrix, sizeof(Matrix));
	DeviceContext->Unmap(DataBuffer.Get(), 0u);
};

} // namespace Haku
