#pragma once
#include "Window.h"
class Application
{
public:
	Application() = default;
	void Run();
private:
	Window AppWindow;
};

