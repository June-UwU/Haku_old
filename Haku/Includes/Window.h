#pragma once
#include <Windows.h>
#include "wil/resource.h"
#include <optional>
#include <string>
#include "Throwables.h"
#include <memory>
#include "Graphics.h"

class Window
{
public:
	Window();
	Window(Window& rhs) = delete;
	Window operator=(const Window&) = delete;
	bool HandleMessages() noexcept;
	Graphics& Gfx() noexcept;
private:
	LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Adapter(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
private:
	wil::unique_hwnd Handle;
	wil::unique_hicon HakuIcon;
	std::unique_ptr<Graphics>GFX;
};