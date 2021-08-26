#include "Model.h"
#include <DirectXMath.h>

#include "ExceptionMacros.h"
#include "Throwables.h"

Model::Model(
	ID3D11Device*		 Device,
	ID3D11DeviceContext* DeviceContext,
	std::vector<int>&&	 Index,
	std::vector<Point>&& Vertex)
	: ClientHeight(801.0f)
	, ClientWidth(1536.0f)
{
	VertexData = std::make_unique<Haku::VertexBuffer>(std::move(Vertex),Device);
	IndexData = std::make_unique<Haku::IndexBuffer>(std::move(Index),Device);
	char FilePath[256];
	GetModuleFileNameA(nullptr, FilePath, std::size(FilePath));
	std::filesystem::path Exe(FilePath);
	Exe.remove_filename();
	const std::filesystem::path VertexShaderPath(Exe / "../../Shaders/VertexShader1.hlsl");
	const std::filesystem::path PixelShaderPath(Exe / "../../Shaders/PixelShader1.hlsl");

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	// Men, shader isn't part of the model
	EXCEPT_HR_THROW(D3DCompileFromFile(
		VertexShaderPath.wstring().c_str(),
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		flags,
		NULL,
		VertexBlob.GetAddressOf(),
		ErrorBlob.GetAddressOf()))

	EXCEPT_HR_THROW(D3DCompileFromFile(
		PixelShaderPath.wstring().c_str(),
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		flags,
		NULL,
		PixelBlob.GetAddressOf(),
		ErrorBlob.GetAddressOf()))

	EXCEPT_HR_THROW(Device->CreateVertexShader(
		VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), nullptr, VertexShader.GetAddressOf()))
	EXCEPT_HR_THROW(Device->CreatePixelShader(
		PixelBlob->GetBufferPointer(), PixelBlob->GetBufferSize(), nullptr, PixelShader.GetAddressOf()))

	

	D3D11_INPUT_ELEMENT_DESC VertexInputDesc[]{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	Matrix = { DirectX::XMMatrixTranspose(
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f) * DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) *
		DirectX::XMMatrixPerspectiveFovLH(90, ClientWidth / ClientHeight, 0.5f, 100.0f)) };
	D3D11_BUFFER_DESC ConstantBuffer{};
	ConstantBuffer.ByteWidth	  = sizeof(Matrix);
	ConstantBuffer.Usage		  = D3D11_USAGE_DYNAMIC;
	ConstantBuffer.BindFlags	  = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA ConstantSubResource{};
	ConstantSubResource.pSysMem = &Matrix;

	EXCEPT_HR_THROW(Device->CreateBuffer(&ConstantBuffer, &ConstantSubResource, RotationMatrix.GetAddressOf()))
	/*---------------*/

	EXCEPT_HR_THROW(Device->CreateInputLayout(
		VertexInputDesc,
		std::size(VertexInputDesc),
		VertexBlob->GetBufferPointer(),
		VertexBlob->GetBufferSize(),
		InputLayout.GetAddressOf()))
}

void Model::Bind(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	VertexData->Bind(DeviceContext);
	IndexData->Bind(DeviceContext);
	DeviceContext->VSSetShader(VertexShader.Get(), 0, 0);
	DeviceContext->PSSetShader(PixelShader.Get(), 0, 0);
	DeviceContext->VSSetConstantBuffers(0u, 1u, RotationMatrix.GetAddressOf());
	DeviceContext->IASetInputLayout(InputLayout.Get());
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT vp;
	vp.Width	= ClientWidth;
	vp.Height	= ClientHeight;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DeviceContext->RSSetViewports(1u, &vp);
	DeviceContext->DrawIndexed(IndexData->GetIndicesNo(), 0, 0);
}
