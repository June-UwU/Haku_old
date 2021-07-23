#include "Graphics.h"
#include "ExceptionMacros.h"
#include <filesystem>
#include <functional>
#include <d3dcompiler.h>
#include <dxgi1_6.h>

#pragma comment(lib ,"DXGI.lib")
#pragma comment(lib ,"d3d11.lib")
#pragma comment(lib ,"dxgi.lib")
#pragma comment(lib ,"d3dCompiler.lib")


Graphics::Graphics(HWND Handle)
{
	DXGI_ADAPTER_DESC1 _Desc;
	Microsoft::WRL::ComPtr<IDXGIFactory1> _Factory;
	Microsoft::WRL::ComPtr<IDXGIAdapter1> _Adapter;
	Microsoft::WRL::ComPtr<IDXGIOutput> _Output;
	//next issue::refresh rate and resolutions
	//Note:: Should Look into enumerating display and graphics adapter
	HAKU_INFO_QUEUE_CHECK_DUMP(CreateDXGIFactory1(__uuidof(IDXGIFactory), reinterpret_cast<void**>(_Factory.GetAddressOf())))
	HAKU_INFO_QUEUE_CHECK_DUMP(_Factory->EnumAdapters1(0, _Adapter.GetAddressOf()))
	HAKU_INFO_QUEUE_CHECK_DUMP(_Adapter->GetDesc1(&_Desc))
	HAKU_LOG_INFO(_Desc.Description);
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

	D3D11_TEXTURE2D_DESC DepthStencilBufferDesc{};
	DepthStencilBufferDesc.Format = DXGI_FORMAT_R32_TYPELESS; //probable error cause..why..?
	//A 32-bit z-buffer format that supports 24 bits for depth and 8 bits for stencil.
	DepthStencilBufferDesc.Width = ClientWidth;
	DepthStencilBufferDesc.Height = ClientHeight;
	DepthStencilBufferDesc.MipLevels = 1;
	DepthStencilBufferDesc.ArraySize = 1;
	DepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilBufferDesc.SampleDesc.Quality = 0;
	DepthStencilBufferDesc.SampleDesc.Count = 1;
	DepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;

	// Depth test parameters
	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF; //16 bits  or 2 bytes..? is it half the buffer..?
	DepthStencilDesc.StencilWriteMask = 0xFF;//check how stencil is implemented

	// Stencil operations if pixel is front-facing
	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	
	UINT flags = 0;
#ifdef _DEBUG
	 flags = D3D11_CREATE_DEVICE_DEBUG ;
#endif 
	HAKU_INFO_QUEUE_CHECK_DUMP(D3D11CreateDeviceAndSwapChain(_Adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN,
		NULL,flags,FeatureLevel,std::size(FeatureLevel), D3D11_SDK_VERSION,&SwapChainDesc,
		_SwapChain.GetAddressOf(),_Device.GetAddressOf(),NULL,_DeviceContext.GetAddressOf()))
	Microsoft::WRL::ComPtr<ID3D11Resource>RenderingBackBuffer;
	_SwapChain->GetBuffer(0, IID_PPV_ARGS(RenderingBackBuffer.ReleaseAndGetAddressOf()));

	HAKU_INFO_QUEUE_CHECK_DUMP(_Device->CreateTexture2D(&DepthStencilBufferDesc, 0, _DepthStencilBuffer.GetAddressOf()))
	HAKU_INFO_QUEUE_CHECK_DUMP(_Device->CreateDepthStencilState(&DepthStencilDesc, _DepthStencilState.GetAddressOf()))
	HAKU_INFO_QUEUE_CHECK_DUMP(_Device->CreateRenderTargetView(RenderingBackBuffer.Get(), nullptr, _RenderTarget.GetAddressOf()))
	_DeviceContext->OMSetDepthStencilState(_DepthStencilState.Get(), 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStenciViewDesc{};
	DepthStenciViewDesc.Format = DXGI_FORMAT_D32_FLOAT;//probable error cause..why..?
	//32-bit depth, 8-bit stencil, and 24 bits are unused
	DepthStenciViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStenciViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	HAKU_INFO_QUEUE_CHECK_DUMP(_Device->CreateDepthStencilView(_DepthStencilBuffer.Get(),&DepthStenciViewDesc,
		_DepthStencilView.GetAddressOf()))

// Bind the depth stencil view
	_DeviceContext->OMSetRenderTargets(1,_RenderTarget.GetAddressOf(),_DepthStencilView.Get());
}

void Graphics::ClearBackBuffer(float Red, float Blue, float Green, float Alpha) noexcept
{
	float Color[4]{Red,Blue,Green,Alpha};
	/*Clearing the render target buffer to a single value...probably to Black{0,0,0,0}*/
	_DeviceContext->ClearRenderTargetView(_RenderTarget.Get(), Color);
	//Since there is no stencil state or view bounded by the RenderTargetView how do i do this..?
}

void Graphics::PresentSwapChainBuffer()
{
	HAKU_INFO_QUEUE_LOG;
	HAKU_INFO_QUEUE_CHECK_DUMP(_SwapChain->Present(0, 0))
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
	

	HAKU_INFO_QUEUE_CHECK_DUMP(D3DCompileFromFile(VertexShaderPath.wstring().c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
		flags, NULL, VertexBlob.GetAddressOf(), ErrorBlob.GetAddressOf()))

	HAKU_INFO_QUEUE_CHECK_DUMP(D3DCompileFromFile(PixelShaderPath.wstring().c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0",
		flags, NULL, PixelBlob.GetAddressOf(), ErrorBlob.GetAddressOf()))
	

	HAKU_INFO_QUEUE_CHECK_DUMP(_Device->CreateVertexShader(VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), nullptr, VertexShader.GetAddressOf()))
	HAKU_INFO_QUEUE_CHECK_DUMP(_Device->CreatePixelShader(PixelBlob->GetBufferPointer(), PixelBlob->GetBufferSize(), nullptr, PixelShader.GetAddressOf()))

	_DeviceContext->VSSetShader(VertexShader.Get(), 0, 0);
	_DeviceContext->PSSetShader(PixelShader.Get(), 0, 0);
	VertexShader.Reset();
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;

	//investigate the XMFLOAT4X4 thing and meshing of that bullshit with XMMATRIX
	struct Rotation
	{
		DirectX::XMMATRIX RotationZ;
	};
	
	//yeah rotations are really fcked
	Rotation Matrix{ 
		
		DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationX(ThetaZ)*
		DirectX::XMMatrixRotationZ(ThetaZ)*
		DirectX::XMMatrixRotationY(ThetaZ)*
		DirectX::XMMatrixScaling((ClientHeight / ClientWidth), (ClientHeight / ClientHeight),(ClientHeight / ClientHeight)))
	};//list initialization works...!!!or does it..!

	D3D11_BUFFER_DESC ConstantBuffer{};
	ConstantBuffer.ByteWidth = sizeof(Matrix);
	ConstantBuffer.Usage = D3D11_USAGE_DYNAMIC;
	ConstantBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	D3D11_SUBRESOURCE_DATA ConstantSubResource{};
	ConstantSubResource.pSysMem = &Matrix;
	
	HAKU_INFO_QUEUE_CHECK_DUMP(_Device->CreateBuffer(&ConstantBuffer, &ConstantSubResource, RotationMatrix.GetAddressOf()))
	_DeviceContext->VSSetConstantBuffers(0u, 1u, RotationMatrix.GetAddressOf());

	struct Vertex
	{
		struct 
		{
			float x;
			float y;
			float z;
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
	{0.5f,  -0.5f,  -0.5f,	    1.0f,0.0f,0.0f}, //0
	{-0.5f,  -0.5f, -0.5f,	    0.0f,1.0f,0.0f}, //1
	{-0.5f, 0.5f,   -0.5f,	    0.0f,0.0f,1.0f}, //2
	{0.5f,  0.5f,   -0.5f,	    1.0f,1.0f,0.0f}, //3

	{-0.5f, -0.5f,  0.5f,	    0.0f,1.0f,1.0f}, //4
	{0.5f,  -0.5f,  0.5f,	    1.0f,1.0f,1.0f}, //5
	{-0.5f, 0.5f,   0.5f,	    1.0f,0.0f,1.0f}, //6
	{0.5f,  0.5f,   0.5f,	    1.0f,0.5f,0.0f}, //7
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

	//This might still fck up and need to be disambiguated

	unsigned int Index[]{ 
		0,2,1,	/* R */	 0,4,5,  /* R */
		0,3,2,	/* R */	 7,4,6,	 /* R */
		7,5,4,	/* R */	 1,2,6,	 /* R */
		1,6,4,	/* R */	 0,5,3,	 /* R */
		3,5,7,	/* R */	 3,7,6,  /* R */
		0,1,4,	/* R */	 3,6,2   /* R */};

	D3D11_BUFFER_DESC IndexBufferDesc{};
	IndexBufferDesc.ByteWidth = sizeof(Index) * std::size(Index);
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA IndexSubRes{0};
	IndexSubRes.pSysMem = Index;

	HAKU_INFO_QUEUE_CHECK_DUMP(_Device->CreateBuffer(&IndexBufferDesc, &IndexSubRes, IndexBuffer.GetAddressOf()))
	HAKU_INFO_QUEUE_CHECK_DUMP(_Device->CreateBuffer(&VertexDesc, &VertexSubRes, VertexBuffer.GetAddressOf()))
	UINT stride = sizeof(Vertex);
	UINT OffSet = 0u;
	_DeviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &OffSet);
	_DeviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
	D3D11_INPUT_ELEMENT_DESC VertexInputDesc[]{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(float)*3,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	HAKU_INFO_QUEUE_CHECK_DUMP(_Device->CreateInputLayout(VertexInputDesc, std::size(VertexInputDesc), VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), InputLayout.GetAddressOf()))
	_DeviceContext->IASetInputLayout(InputLayout.Get());
	

	_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_DeviceContext->OMSetRenderTargets(1, _RenderTarget.GetAddressOf(), 0);

	D3D11_VIEWPORT vp;
	vp.Width = ClientWidth;
	vp.Height = ClientHeight;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_DeviceContext->RSSetViewports(1, &vp);
	_DeviceContext->DrawIndexed(std::size(Index), 0, 0);

}
