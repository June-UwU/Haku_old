#include "Model.h"

Model::Model(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, std::vector<int>&& Index, std::vector<Point>&& Vertex)
	:ClientHeight(801.0f),
	ClientWidth(1536.0f)
{
	Indexdata = Index;
	VertexData = Vertex;
	char FilePath[256];
	GetModuleFileNameA(nullptr, FilePath, std::size(FilePath));
	std::filesystem::path Exe(FilePath);
	Exe.remove_filename();
	std::filesystem::path VertexShaderPath(Exe / "../../Shaders/VertexShader1.hlsl");
	std::filesystem::path PixelShaderPath(Exe / "../../Shaders/PixelShader1.hlsl");

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

	D3D11_BUFFER_DESC VertexDesc{};
	VertexDesc.ByteWidth		   = VertexData.size() * sizeof(Point);
	VertexDesc.Usage			   = D3D11_USAGE_DEFAULT;
	VertexDesc.BindFlags		   = D3D11_BIND_VERTEX_BUFFER;
	VertexDesc.CPUAccessFlags	   = 0;
	VertexDesc.MiscFlags		   = 0;
	VertexDesc.StructureByteStride = sizeof(Point);

	D3D11_SUBRESOURCE_DATA VertexSubRes{};
	VertexSubRes.pSysMem = VertexData.data();

	D3D11_BUFFER_DESC IndexBufferDesc{};
	IndexBufferDesc.ByteWidth	   = sizeof(int) * Indexdata.size();
	IndexBufferDesc.Usage		   = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.BindFlags	   = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags	   = 0;

	D3D11_SUBRESOURCE_DATA IndexSubRes{ 0 };
	IndexSubRes.pSysMem = Indexdata.data();

	EXCEPT_HR_THROW(Device->CreateBuffer(&IndexBufferDesc, &IndexSubRes, IndexBuffer.GetAddressOf()))
	EXCEPT_HR_THROW(Device->CreateBuffer(&VertexDesc, &VertexSubRes, VertexBuffer.GetAddressOf()))

	D3D11_INPUT_ELEMENT_DESC VertexInputDesc[]{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	Matrix = { DirectX::XMMatrixTranspose(
		DirectX::XMMatrixTranslation(0.0f,0.0f,4.0f) * DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) *
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
	DeviceContext->VSSetShader(VertexShader.Get(), 0, 0);
	DeviceContext->PSSetShader(PixelShader.Get(), 0, 0);
	UINT stride = sizeof(Point);
	UINT OffSet = 0u;
	DeviceContext->VSSetConstantBuffers(0u, 1u, RotationMatrix.GetAddressOf());
	DeviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &OffSet);
	DeviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
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
	DeviceContext->DrawIndexed(Indexdata.size(), 0, 0);
}
