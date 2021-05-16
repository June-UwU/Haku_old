/*the fck happened here
this thing is cucking me for the second time*/
#pragma once
#include <windows.h>
#include <string>

class Throwables : std::exception
{
public:
	Throwables(int Line, std::string File, std::string Function, HRESULT Code) noexcept;
	const char* what() noexcept;
private:
	int Line;
	std::string Function;
	std::string File;
	HRESULT Code;
	std::string OutputString;
};

