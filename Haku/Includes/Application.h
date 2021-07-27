#pragma once
#include "Window.h"
#include <chrono>
#include "HakuLog.h"
class Application
{
public:
	Application() = default;
	void Run();
private:
	Window AppWindow;
	float Rotate = 0.0f;
	float ZBuffer = 0.9f;
};

