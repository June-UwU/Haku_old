#include "Window.h"
#include <filesystem>

Window::Window()
{
    // Register the window class.
    const char CLASS_NAME[] = "Haku Window Class";
    int argc;
    LPWSTR* CommandLine = CommandLineToArgvW(GetCommandLineW(), &argc);
    HakuIcon.reset(nullptr);
    if (CommandLine)
    {
        std::filesystem::path ExecutablePath{ CommandLine[0] };
        auto IconPath = ExecutablePath.parent_path()/"Haku.ico";
        HakuIcon.reset(reinterpret_cast<HICON>(LoadImage(nullptr, IconPath.string().data(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE)));
        LocalFree(CommandLine);
    }
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
    SetWindowLongPtrA(Handle.get(), GWLP_USERDATA, (LONG_PTR)this);
}

bool Window::HandleMessages() noexcept
{

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (msg.message == WM_DESTROY)
        {
            return false ;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return true;
    }
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


