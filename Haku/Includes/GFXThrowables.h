#pragma once
#include <string>
#include <dxgi.h>

class GFXThrowables : std::exception
{
public:
	GFXThrowables(int Line,const char* File,const char*Function,HRESULT Hr)noexcept;
	const char* what() noexcept;
private:	
	std::string ErrorMessage;
};

