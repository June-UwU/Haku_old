#include "Graphics.h"
#include "ExceptionMacros.h"
#include <filesystem>
#include <functional>
#include <d3dcompiler.h>


#pragma comment(lib ,"d3d11.lib")
#pragma comment(lib ,"dxgi.lib")
#pragma comment(lib ,"d3dCompiler.lib")


Graphics::Graphics(HWND Handle)
{
	//next issue::refresh rate and resolutions
	//Note:: Should Look into enumerating display and graphics adapter
	RECT ThrowAway;
	GetClientRect(Handle, &ThrowAway);
	//Aspect ratio set value..meh
	ClientHeight = ThrowAway.bottom - ThrowAway.top;
	ClientWidth = ThrowAway.right - ThrowAway.left;
	
	D3D_FEATURE_LEVEL FeatureLevel[]{ 
		D3D_FEATURE_LEVEL_11_0
	};
	/*SwapChainDesc need Client rect..*/
	DXGI_SWAP_CHAIN_DESC SwapChainDesc{};
	SwapChainDesc.BufferDesc.Width = 0;
	SwapChainDesc.BufferDesc.Height = 0;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = Handle;
	SwapChainDesc.Windowed = 1;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = 0;

	UINT flags = 0;
#ifdef _DEBUG
	 flags = D3D11_CREATE_DEVICE_DEBUG ;
#endif 
	 EXCEPT_HR_THROW(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, flags, FeatureLevel, 1,
		D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, NULL, &DeviceContext))

	Microsoft::WRL::ComPtr<ID3D11Resource>RenderingBackBuffer;
	SwapChain->GetBuffer(0, IID_PPV_ARGS(RenderingBackBuffer.ReleaseAndGetAddressOf()));

	EXCEPT_HR_THROW(Device->CreateRenderTargetView(RenderingBackBuffer.Get(), nullptr, RenderTarget.GetAddressOf()))
}

void Graphics::ClearBackBuffer(float Red, float Blue, float Green, float Alpha) noexcept
{
	float Color[4]{Red,Blue,Green,Alpha};
	/*Clearing the render target buffer to a single value...probably to Black{0,0,0,0}*/
	DeviceContext->ClearRenderTargetView(RenderTarget.Get(), Color);
}

void Graphics::PresentSwapChainBuffer()
{
	HAKU_INFO_QUEUE_LOG;
	EXCEPT_HR_THROW(SwapChain->Present(0, 0))
}

void Graphics::Tinkering(float ThetaZ)
{
	char FilePath[256];
	GetModuleFileNameA(nullptr, FilePath, std::size(FilePath));
	std::filesystem::path Exe(FilePath);
	Exe.remove_filename();
	std::filesystem::path VertexShaderPath(Exe / "../../Shaders/VertexShader.hlsl");
	std::filesystem::path PixelShaderPath(Exe / "../../Shaders/PixelShader.hlsl");

	Microsoft::WRL::ComPtr<ID3DBlob> ErrorBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> VertexBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> PixelBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;
	Microsoft::WRL::ComPtr< ID3D11Buffer> RotationMatrix;

	
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	//done a preprosser conditional to avoid runtime check ... might need if we support costom shaders
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	EXCEPT_HR_THROW(D3DCompileFromFile(VertexShaderPath.wstring().c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
		flags, NULL, VertexBlob.GetAddressOf(), ErrorBlob.GetAddressOf()));

	
	D3DCompileFromFile(PixelShaderPath.wstring().c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0",
		flags, NULL, PixelBlob.GetAddressOf(), ErrorBlob.GetAddressOf());
	

	Device->CreateVertexShader(VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), nullptr, VertexShader.GetAddressOf());
	Device->CreatePixelShader(PixelBlob->GetBufferPointer(), PixelBlob->GetBufferSize(), nullptr, PixelShader.GetAddressOf());

	DeviceContext->VSSetShader(VertexShader.Get(), 0, 0);
	DeviceContext->PSSetShader(PixelShader.Get(), 0, 0);
	VertexShader.Reset();
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;

	//investigate the XMFLOAT4X4 thing and meshing of that bullshit with XMMATRIX
	struct Rotation
	{
		DirectX::XMMATRIX RotationZ;
	};

	//yeah rotations are really fcked
	Rotation Matrix{ DirectX::XMMatrixRotationY(ThetaZ) };//list initialization works...!!!or does it..!

	D3D11_BUFFER_DESC ConstantBuffer{};
	ConstantBuffer.ByteWidth = sizeof(Matrix);
	ConstantBuffer.Usage = D3D11_USAGE_DYNAMIC;
	ConstantBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	D3D11_SUBRESOURCE_DATA ConstantSubResource{};
	ConstantSubResource.pSysMem = &Matrix;
	
	Device->CreateBuffer(&ConstantBuffer, &ConstantSubResource, RotationMatrix.GetAddressOf());
	DeviceContext->VSSetConstantBuffers(0u, 1u, RotationMatrix.GetAddressOf());

	struct Vertex
	{
		struct 
		{
			float x;
			float y;
		}pos;
		struct
		{
			float Red;
			float Blue;
			float Green;
		}RGB;
	};
	Vertex Vertices[]
	{
		{-0.5f,0.5f,1.0f,0.0f,0.0f},
		{0.5f,0.5f,0.0f,1.0f,0.0f},
		{0.5f,-0.5f,0.0f,0.0f,1.0f},
		{-0.5f,-0.5f,1.0f,0.0f,1.0f}
	};

	D3D11_BUFFER_DESC VertexDesc{};
	VertexDesc.ByteWidth = sizeof(Vertices);
	VertexDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexDesc.CPUAccessFlags = 0;
	VertexDesc.MiscFlags = 0;
	VertexDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA VertexSubRes{};
	VertexSubRes.pSysMem = Vertices;

	unsigned int Index[]{ 0,1,2,
						  0,2,3};

	D3D11_BUFFER_DESC IndexBufferDesc{};
	IndexBufferDesc.ByteWidth = sizeof(Index) * std::size(Index);
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA IndexSubRes{0};
	IndexSubRes.pSysMem = Index;

	Device->CreateBuffer(&IndexBufferDesc, &IndexSubRes, IndexBuffer.GetAddressOf());
	Device->CreateBuffer(&VertexDesc, &VertexSubRes, VertexBuffer.GetAddressOf());
	UINT stride = sizeof(Vertex);
	UINT OffSet = 0u;
	DeviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &OffSet);
	DeviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
	D3D11_INPUT_ELEMENT_DESC VertexInputDesc[]{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(float)*2,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	Device->CreateInputLayout(VertexInputDesc, std::size(VertexInputDesc), VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), InputLayout.GetAddressOf());
	DeviceContext->IASetInputLayout(InputLayout.Get());
	

	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->OMSetRenderTargets(1, RenderTarget.GetAddressOf(), 0);

	D3D11_VIEWPORT vp;
	vp.Width = ClientWidth;
	vp.Height = ClientHeight;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DeviceContext->RSSetViewports(1, &vp);

	
	DeviceContext->DrawIndexed(std::size(Index), 0, 0);
}
