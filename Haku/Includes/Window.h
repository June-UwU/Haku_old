#pragma once
#include <Windows.h>
#include "wil/resource.h"
#include <optional>

class Window
{
public:
	Window();
	bool HandleMessages() noexcept;
private:
	LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Adapter(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
private:
	wil::unique_hwnd Handle;
	wil::unique_hicon HakuIcon;
};