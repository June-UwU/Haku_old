#pragma once
#include "Window.h"
#include <chrono>
class Application
{
public:
	Application() = default;
	void Run();
private:
	Window AppWindow;
};

