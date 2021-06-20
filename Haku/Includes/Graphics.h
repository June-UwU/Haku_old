#pragma once
#include "Throwables.h"

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
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget;
};

