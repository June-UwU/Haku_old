#include "Window.h"
#include <filesystem>
#include "ExceptionMacros.h"
#include "HakuLog.h"

Window::Window()
{
    // Register the window class.
    const char CLASS_NAME[] = "Haku Window Class";
    char Filepath[256];
    GetModuleFileName(nullptr, Filepath, std::size(Filepath));

    std::filesystem::path ExecutablePath(Filepath);
    auto IconPath = ExecutablePath.parent_path()/"../Haku.ico";
    HakuIcon.reset(reinterpret_cast<HICON>(LoadImage(nullptr, IconPath.string().data(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE)));
         
    WNDCLASS wc = { };
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = Adapter;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = HakuIcon.get();
    RegisterClass(&wc);

    /// <summary>
    /// So WS_MAXIMIZE|WS_OVERLAPPED GIVES A "VEDIO GAME FULLSCREEN"
    /// </summary>

    Handle.reset(CreateWindowExA(NULL,
        CLASS_NAME,"Haku",WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|WS_VISIBLE,0,0,1280,720,
        nullptr,nullptr,GetModuleHandle(nullptr),this));
    HAKU_CONSOLE_INIT;
    HAKU_LOG_INIT;
    /*should set a pattern...?*/
    
    HAKU_LOG_INFO("info");
    HAKU_LOG_WARN("warn");
    HAKU_LOG_CRIT("crit");
    HAKU_LOG_ERR("error");
    
    if (!Handle)
    {
        throw EXCEPT_LAST_THROW();
    }
    GFX = std::make_unique<Graphics>(Handle.get());
    SetWindowLongPtrA(Handle.get(), GWLP_USERDATA, (LONG_PTR)this);
}
/*Returns Loopp Control Variable*/
/*Figure out a way to cleanly return exit code..*/
bool Window::HandleMessages() noexcept
{
    MSG msg = { };
    while (true)
    {
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);//peek message has a return of bool thats goes false after no messages
        if (msg.message == WM_QUIT)
        {
            return false ;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return true;
    }
}

Graphics& Window::Gfx() noexcept
{
    return *GFX;
}

LRESULT Window::WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT Ret{};
    switch (message)
    {
    case WM_CLOSE:
    {
        if (MessageBox(handle, "Really quit?", "Haku-Chan here..!", MB_OKCANCEL) == IDOK)
        {
            PostQuitMessage(0);
        }
    }break;
    default:
    {
        Ret = DefWindowProcA(handle, message, wParam, lParam);
    }
    }
    return Ret;
}

LRESULT Window::Adapter(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    //CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    Window* HandleWindow = reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA));
    return HandleWindow->WindowProc(handle, message, wParam, lParam);
    return DefWindowProcA(handle, message, wParam, lParam);
}


