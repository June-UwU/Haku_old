#pragma once
#include "Throwables.h"
#include "DXGIInfoQueue.h"
#include <wrl.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

class Graphics
{
	friend class AssetManager;

public:
	Graphics(HWND handle);
	Graphics(Graphics& rhs) = delete;
	~Graphics();
	Graphics operator=(const Graphics& rhs) = delete;
	void	 ClearBackBuffer(float Red, float Blue, float Green, float Alpha) noexcept;
	void	 OnWindowResize(HWND Handle);
	void	 PresentSwapChainBuffer();
	void	 Tinkering(float ThetaZ, float Translation);
	void	 SetViewPorts() noexcept;
	float	 ClientHeight;
	float	 ClientWidth;

private:
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D11Debug> _Debugger;
	DXGIInfoQueue						InfoQueue;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device>			_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		_DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11Resource>			_RenderingBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	_RenderTarget;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			_DepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _DepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	_DepthStencilView;
};

#if defined _DEBUG
#define HAKU_LIVE_OBJECT	_Debugger->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL)
#define HAKU_INFO_QUEUE_LOG InfoQueue.log_message();
#define HAKU_INFO_QUEUE_CHECK_DUMP(Result)                                                                             \
	if (Result != S_OK)                                                                                                \
	{                                                                                                                  \
		InfoQueue.log_message();                                                                                       \
		GFX_EXCEPT_HR_THROW(Result)                                                                                    \
	}
#else
#define HAKU_INFO_QUEUE_LOG
#define HAKU_INFO_QUEUE_CHECK_DUMP(Result)                                                                             \
	if (Result != S_OK)                                                                                                \
	{                                                                                                                  \
		GFX_EXCEPT_HR_THROW(Result)                                                                                    \
	}
#endif
