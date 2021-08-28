#include "Application.h"
#include <filesystem>
#include <string>
void Application::Run()
{
	Manager.SetGraphics(&AppWindow.Gfx());
	char FilePath[256];
	GetModuleFileNameA(nullptr, FilePath, std::size(FilePath));
	std::filesystem::path Exe(FilePath);
	Exe.remove_filename();
	//std::filesystem::path ModelPath(Exe / "../../Model/Medic/Medical worker.obj");
	std::filesystem::path ModelPath(Exe / "../../Model/suzanne.obj");
	std::string path = ModelPath.string();
	Manager.ReadModel(path);
	while (AppWindow.HandleMessages())
	{
		// Clearing The back buffer on each cycle of event,apparently clearing doesn't present the damn frame..
		// this is equivalent to wiping the board clear
		// HAKU_LOG_INFO("BackBuffer Clearing");
		AppWindow.Gfx().ClearBackBuffer(0.0f, 0.0f, 0.0f, 1.0f);
		Manager.Draw();
		AppWindow.Gfx().PresentSwapChainBuffer();
		if (AppWindow.Mouse.LeftDown)
		{
			TestModifier.XRotate += 0.5f;
		}
		if (AppWindow.KeyBoard.CheckKeyDown(VK_UP))
		{
			TestModifier.ZTrans += 0.5f;
		}
		if (AppWindow.KeyBoard.CheckKeyDown(VK_DOWN))
		{
			TestModifier.ZTrans -= 0.5f;	
		}
		Manager.UpdateTestData(TestModifier);
		AppWindow.SetTrigger();
	}
}
