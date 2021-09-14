#pragma once
#include "GPUDataType.h"
#include "Buffer.h"

namespace Haku
{
class Lights
{
public:
private:
	LightingConst BufferValue;
	PixelConstBuffer<LightingConst> ConstBuffer;
};
} // namespace Haku
