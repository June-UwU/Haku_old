#include "wil/wrl.h"
#include <d3d11.h>

namespace Haku
{
class InputLayout
{
public:
	InputLayout(ID3D11Device* Device, ID3DBlob* VertexBlobPointer, size_t VertexBlobSize);
	void Bind(ID3D11DeviceContext* DeviceContext) noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> _InputLayout;
};
} // namespace Haku
