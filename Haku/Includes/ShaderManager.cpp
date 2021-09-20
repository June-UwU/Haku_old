#include "ShaderManager.h"
#include <exception>

void ShaderManager::AddVertexShader(ID3D11Device* Device, const wchar_t* Path, int Key)
{
	VertexShaderMap.emplace(Key, Haku::VertexShader(Path, Device));
}

void ShaderManager::AddPixelShader(ID3D11Device* Device, const wchar_t* Path, int Key)
{
	PixelShaderMap.emplace(Key, Haku::PixelShader(Path, Device));
}

void ShaderManager::ShaderSelect(ID3D11DeviceContext* DeviceContext, int Key)
{
	if (VertexShaderMap.find(Key) != VertexShaderMap.end())
	{
		VertexShaderMap[Key].Bind(DeviceContext);
	}
	else
	{
		throw std::exception("Invalid Vertex Shader");
	}
	if (PixelShaderMap.find(Key) != PixelShaderMap.end())
	{
		PixelShaderMap[Key].Bind(DeviceContext);
	}
	else
	{
		throw std::exception("Invalid Vertex Shader");
	}
}
