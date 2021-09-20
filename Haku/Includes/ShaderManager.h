#include "Shaders.h"
#include <unordered_map>

class ShaderManager
{
public:
	ShaderManager() = default;
	void AddVertexShader(ID3D11Device* Device, const wchar_t* Path, int Key);
	void AddPixelShader(ID3D11Device* Device, const wchar_t* Path, int Key);
	void ShaderSelect(ID3D11DeviceContext* DeviceContext, int Key);

private:
	std::unordered_map<int, Haku::VertexShader> VertexShaderMap;
	std::unordered_map<int, Haku::PixelShader>	PixelShaderMap;
};
