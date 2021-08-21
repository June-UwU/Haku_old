#pragma once
#include "Window.h"
#include "HakuLog.h"
/*TEMP INCLUDED*/
#include "AssetManager.h"
/*-------------*/
#include <chrono>
class Application
{
public:
	Application() = default;
	void Run();

private:
	Window AppWindow;
	AssetManager Manager;
	float  Rotate  = 0.0f;
	float  ZBuffer = 0.9f;
};
