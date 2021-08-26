#pragma once
#include "GPUDataType.h"

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

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(std::vector<Point>&& VertexData, ID3D11Device* Device);
	virtual void Bind(ID3D11DeviceContext* DeviceContext) override;

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
	VertexConstBuffer(){};
	virtual void Bind(ID3D11DeviceContext* DeviceContext) override;
private:
};
} // namespace Haku
