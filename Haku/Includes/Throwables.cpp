#include "Throwables.h"
#include <sstream>

Throwables::Throwables(int Line, std::string File, std::string Function, HRESULT Code) noexcept
	: Line(Line)
	, File(File)
	, Function(Function)
{
}

const char* Throwables::what() noexcept
{
	char* ErrorString{};
	/*Error string is not translating the HRESULT code*/
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		0,
		Code,
		LANG_SYSTEM_DEFAULT,
		LPSTR(&ErrorString),
		0,
		0);
	if (!ErrorString)
	{
		ErrorString = "Haku-Chan asked windows but got no answer";
	}
	std::ostringstream Formatter;
	Formatter << "Haku-Chan failed..!" << std::endl
			  << "[FILE]:" << File << std::endl
			  << "[FUNCTION]:" << Function << std::endl
			  << "[LINE]:" << Line << std::endl
			  << "[ERROR]:" << ErrorString;
	OutputString = Formatter.str();
	return OutputString.c_str();
}
