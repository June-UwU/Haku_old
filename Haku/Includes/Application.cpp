#include "Application.h"
#include <filesystem>
#include <string>
// iterations is too fast for now have to ad
// upon inspection iterations doesn't seem to be fast enough to produce tearing or cliping
void Application::Run()
{
	Manager.SetGraphics(&AppWindow.Gfx());
	char FilePath[256];
	GetModuleFileNameA(nullptr, FilePath, std::size(FilePath));
	std::filesystem::path Exe(FilePath);
	Exe.remove_filename();
	std::filesystem::path ModelPath(Exe / "../../Model/Medic/Medical worker.obj");
	std::string path = ModelPath.string();
	//Manager.ReadModel(path,ZBuffer);
	Manager.Test(Rotate,ZBuffer);
	while (AppWindow.HandleMessages())
	{
		// Clearing The back buffer on each cycle of event,apparently clearing doesn't present the damn frame..
		// this is equivalent to wiping the board clear
		// HAKU_LOG_INFO("BackBuffer Clearing");
		AppWindow.Gfx().ClearBackBuffer(0.0f, 0.0f, 0.0f, 1.0f);
		Manager.Draw(Rotate,ZBuffer);
		AppWindow.Gfx().PresentSwapChainBuffer();
		if (AppWindow.Mouse.LeftDown)
		{
			Rotate += 0.01f;
		}
		if (AppWindow.KeyBoard.CheckKeyDown(VK_UP))
		{
			ZBuffer += 0.1f;
		}
		if (AppWindow.KeyBoard.CheckKeyDown(VK_DOWN))
		{
			ZBuffer -= 0.1f;
		}
		AppWindow.SetTrigger();
	}
}
