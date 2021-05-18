#include "Application.h"

void Application::Run()
{
	while (AppWindow.HandleMessages())
	{
		//Clearing The back buffer on each cycle of event,apparently clearing doesn't present the damn frame..
		//this is equivalent to wiping the board clean
		AppWindow.Gfx().ClearBackBuffer(0.0f, 0.0f,0.0f, 1.0f);

		//Present the Created frame
		AppWindow.Gfx().PresentSwapChainBuffer();
	}
}
