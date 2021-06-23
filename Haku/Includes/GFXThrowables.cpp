#include "GFXThrowables.h"
#include <DXGItype.h>
#include <sstream>
#include <stdlib.h>

GFXThrowables::GFXThrowables(int Line, const char* File, const char* Function, HRESULT Hr) noexcept
{
	char* _ErrorDesc{};
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, Hr,
		LANG_SYSTEM_DEFAULT, _ErrorDesc, 0, nullptr);
	if (!_ErrorDesc)
	{
		_ErrorDesc = "Haku-Chan asked Windows and got nothing ";
	}
	std::ostringstream Formatter;
	Formatter << "Haku-Chan failed..!" << std::endl
		<< "[FILE]:" << File << std::endl
		<< "[FUNCTION]:" << Function << std::endl
		<< "[LINE]:" << Line << std::endl
		<< "[ERROR]:" << _ErrorDesc;
	ErrorMessage.append(Formatter.str());
}

const char* GFXThrowables::what() noexcept
{
	return ErrorMessage.c_str();
}
