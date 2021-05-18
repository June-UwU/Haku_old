#include "Graphics.h"
#pragma comment(lib ,"d3d11.lib")
#pragma comment(lib ,"dxgi.lib")

Graphics::Graphics(HWND Handle)
{
	D3D_FEATURE_LEVEL FeatureLevel[]{ 
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_10_0,
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


	HRESULT Code = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, FeatureLevel, 3,
		D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, NULL, &DeviceContext);
	Microsoft::WRL::ComPtr<ID3D11Resource>RenderingBackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &RenderingBackBuffer);


	Device->CreateRenderTargetView(RenderingBackBuffer.Get(), nullptr, &RenderTarget);

	RenderingBackBuffer->Release();
}

void Graphics::ClearBackBuffer(float Red, float Blue, float Green, float Alpha)
{
	float Color[4]{Red,Blue,Green,Alpha};
	/*Clearing the render target buffer to a single value...probably to Black{0,0,0,0}*/
	DeviceContext->ClearRenderTargetView(RenderTarget.Get(), Color);
}

void Graphics::PresentSwapChainBuffer()
{
	SwapChain->Present(0, 0);
}
