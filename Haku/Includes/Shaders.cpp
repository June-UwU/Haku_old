#include "Shaders.h"
#include "ExceptionMacros.h"
#include "Throwables.h"
#include <d3dcompiler.h>

Haku::VertexShader::VertexShader(const wchar_t* Path, ID3D11Device* Device)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	// Men, shader isn't part of the model
	EXCEPT_HR_THROW(D3DCompileFromFile(
		Path,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		flags,
		NULL,
		VertexBlob.GetAddressOf(),
		ErrorBlob.GetAddressOf()))
	EXCEPT_HR_THROW(Device->CreateVertexShader(
		VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), nullptr, _VertexShader.GetAddressOf()))
}

void Haku::VertexShader::Bind(ID3D11DeviceContext* DeviceContext) noexcept
{
	DeviceContext->VSSetShader(_VertexShader.Get(), 0, 0);
}

Haku::PixelShader::PixelShader(const wchar_t* Path, ID3D11Device* Device)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	EXCEPT_HR_THROW(D3DCompileFromFile(
		Path,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		flags,
		NULL,
		PixelBlob.GetAddressOf(),
		ErrorBlob.GetAddressOf()))

	EXCEPT_HR_THROW(Device->CreatePixelShader(
		PixelBlob->GetBufferPointer(), PixelBlob->GetBufferSize(), nullptr, _PixelShader.GetAddressOf()))
}

void Haku::PixelShader::Bind(ID3D11DeviceContext* DeviceContext) noexcept
{
	DeviceContext->PSSetShader(_PixelShader.Get(), 0, 0);
}
