#pragma once
#include "Throwables.h"
#include "DXGIInfoQueue.h"
#include <wrl.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>


class Graphics
{
public:
	Graphics(HWND handle);
	Graphics(Graphics& rhs) = delete;
	Graphics operator=(const Graphics& rhs) = delete;
	void ClearBackBuffer(float Red, float Blue, float Green, float Alpha) noexcept;
	void PresentSwapChainBuffer();
	void Tinkering(float ThetaZ);
private:
	float ClientHeight;
	float ClientWidth;
#ifdef _DEBUG
	DXGIInfoQueue InfoQueue;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> _Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> _SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _RenderTarget;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _DepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _DepthStencilState;;
};

#if defined _DEBUG
#define HAKU_INFO_QUEUE_LOG InfoQueue.log_message();
#define HAKU_INFO_QUEUE_CHECK_DUMP(Result) if(Result!=S_OK){ InfoQueue.log_message(); GFX_EXCEPT_HR_THROW(Result) } 
#else
#define HAKU_INFO_QUEUE_LOG 
#define HAKU_INFO_QUEUE_CHECK_DUMP(Result) if(Result!=S_OK){ GFX_EXCEPT_HR_THROW(Result) }
#endif