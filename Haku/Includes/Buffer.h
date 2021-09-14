#pragma once
#include "GPUDataType.h"
#include "ExceptionMacros.h"
#include "Throwables.h"
#include "wil/wrl.h"
#include <vector>
#include <d3d11.h>

namespace Haku
{
class Buffer
{
public:
	virtual void Bind(ID3D11DeviceContext* DeviceContext) = 0;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> DataBuffer;
};
template<class Type>
class VertexBuffer : public Buffer
{
public:
	VertexBuffer(std::vector<Type>&& VertexData, ID3D11Device* Device)
		: stride(sizeof(Point))
		, OffSet(0)
	{
		D3D11_BUFFER_DESC VertexDesc{};
		VertexDesc.ByteWidth		   = VertexData.size() * sizeof(Type);
		VertexDesc.Usage			   = D3D11_USAGE_DEFAULT;
		VertexDesc.BindFlags		   = D3D11_BIND_VERTEX_BUFFER;
		VertexDesc.CPUAccessFlags	   = 0;
		VertexDesc.MiscFlags		   = 0;
		VertexDesc.StructureByteStride = sizeof(Point);

		D3D11_SUBRESOURCE_DATA VertexSubRes{};
		VertexSubRes.pSysMem = VertexData.data();

		EXCEPT_HR_THROW(Device->CreateBuffer(&VertexDesc, &VertexSubRes, DataBuffer.GetAddressOf()))
	};
	virtual void Bind(ID3D11DeviceContext* DeviceContext) override
	{
		DeviceContext->IASetVertexBuffers(0, 1, DataBuffer.GetAddressOf(), &stride, &OffSet);
	};

private:
	UINT stride;
	UINT OffSet;
};

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(std::vector<int>&& IndexData, ID3D11Device* Device);
	virtual void Bind(ID3D11DeviceContext* DeviceContext) override;
	const int	 GetIndicesNo() noexcept;

private:
	const int BufferSize;
};

class VertexConstBuffer : public Buffer
{
public:
	VertexConstBuffer(ID3D11Device* Device, const float ClientWidth, const float ClientHeight);
	virtual void Bind(ID3D11DeviceContext* DeviceContext) override;
	void		 UpdateParameters(ID3D11DeviceContext* DeviceContext, ConstVertexModifer* Reference) noexcept;

private:
	float ClientHeight;
	float ClientWidth;
};
template<typename T>
class PixelConstBuffer : public Buffer
{
public:
	PixelConstBuffer(ID3D11Device* Device, T& Param)
	{
		D3D11_BUFFER_DESC ConstantBuffer{};
		ConstantBuffer.ByteWidth	  = sizeof(Param);
		ConstantBuffer.Usage		  = D3D11_USAGE_DYNAMIC;
		ConstantBuffer.BindFlags	  = D3D11_BIND_CONSTANT_BUFFER;
		ConstantBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA ConstantSubResource{};
		ConstantSubResource.pSysMem = Param;

		EXCEPT_HR_THROW(Device->CreateBuffer(&ConstantBuffer, &ConstantSubResource, DataBuffer.GetAddressOf()))
	}
	virtual void Bind(ID3D11DeviceContext* DeviceContext) override
	{
		DeviceContext->PSSetConstantBuffers(0u, 1u, DataBuffer.GetAddressOf());
	}

private:
};
} // namespace Haku
