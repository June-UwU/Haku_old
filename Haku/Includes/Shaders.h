#pragma once
#include "wil/wrl.h"
#include <d3d11.h>

namespace Haku
{
class PixelShader
{
public:
	PixelShader(const wchar_t* Path, ID3D11Device* Device);
	void Bind(ID3D11DeviceContext* DeviceContext) noexcept;

private:
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3DBlob> ErrorBlob;
#endif
	Microsoft::WRL::ComPtr<ID3DBlob>		  PixelBlob;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> _PixelShader;
};

class VertexShader
{
public:
	VertexShader(const wchar_t* Path, ID3D11Device* Device);
	void Bind(ID3D11DeviceContext* DeviceContext) noexcept;
private:
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3DBlob>		   ErrorBlob;
#endif
	Microsoft::WRL::ComPtr<ID3DBlob>		   VertexBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> _VertexShader;
};
} // namespace Haku
