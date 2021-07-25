#include "Application.h"
#include <string>
//iterations is too fast for now have to ad
//upon inspection iterations doesn't seem to be fast enough to produce tearing or cliping
void Application::Run()
{
	while (AppWindow.HandleMessages())
	{
		//Clearing The back buffer on each cycle of event,apparently clearing doesn't present the damn frame..
		//this is equivalent to wiping the board clear
		//HAKU_LOG_INFO("BackBuffer Clearing");
		AppWindow.Gfx().ClearBackBuffer(0.0f, 0.0f,0.0f, 1.0f);
		
		AppWindow.Gfx().Tinkering(Rotate);
		//AppWindow.Gfx().Tinkering(-Rotate);
		AppWindow.Gfx().PresentSwapChainBuffer();
		if (AppWindow.Mouse.LeftDown)
		{
			Rotate += 0.01f;
		}
		AppWindow.SetTrigger();
		//HAKU_LOG_INFO("Frame Time");
	}
}


