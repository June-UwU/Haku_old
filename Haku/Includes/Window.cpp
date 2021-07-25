#include "Window.h"
#include "ExceptionMacros.h"
#include "HakuLog.h"
#include <filesystem>

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
    HAKU_LOG_INFO("Hello World");
    if (!Handle)
    {
        throw EXCEPT_LAST_THROW();
    }
    HAKU_LOG_INFO("Initailzing Graphics Object");
    GFX = std::make_unique<Graphics>(Handle.get());
    SetWindowLongPtrA(Handle.get(), GWLP_USERDATA, (LONG_PTR)this);
}
/*Returns Loop Control Variable*/
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
        HAKU_LOG_INFO("Close Message");
        if (MessageBox(handle, "Really quit?", "Haku-Chan here..!", MB_OKCANCEL) == IDOK)
        {
            PostQuitMessage(0);
        }
    }break;
    case WM_RBUTTONDOWN:
    {
        POINTS Coordinates = MAKEPOINTS(lParam);
        Mouse.OnMouseMove(Coordinates.x, Coordinates.y);
        HAKU_LOG_INFO("Mouse Right Down", Coordinates.x, Coordinates.y);
        Mouse.RightDown = true;
    }break;
    case WM_RBUTTONUP:
    {
        POINTS Coordinates = MAKEPOINTS(lParam);
        Mouse.OnMouseMove(Coordinates.x, Coordinates.y);
        HAKU_LOG_INFO("Mouse Right Up", Coordinates.x, Coordinates.y);
        Mouse.RightDown = false;
    }break;
    case WM_LBUTTONUP:
    {
        POINTS Coordinates = MAKEPOINTS(lParam);
        Mouse.OnMouseMove(Coordinates.x, Coordinates.y);
        HAKU_LOG_INFO("Mouse Left Up", Coordinates.x, Coordinates.y);
        Mouse.LeftDown = false;
    }break;
    case WM_LBUTTONDOWN:
    {
        POINTS Coordinates = MAKEPOINTS(lParam);
        Mouse.OnMouseMove(Coordinates.x, Coordinates.y);
        HAKU_LOG_INFO("Mouse Left Down", Coordinates.x, Coordinates.y);
        Mouse.LeftDown = true;
    }break;
    case WM_MOUSEMOVE:
    {
        /*
        * THESE BICH DON"T WORK
        *GET_X_LPARAM(lParam)
        *GET_Y_LPARAM(lParam)
        */
        POINTS Coordinates  = MAKEPOINTS(lParam);
        Mouse.OnMouseMove(Coordinates.x, Coordinates.y);
        HAKU_LOG_INFO("Mouse Moved", Coordinates.x, Coordinates.y);
        if (!MK_LBUTTON)
        {
            Mouse.LeftDown = true;
        }
        else
        {
            Mouse.LeftDown = false;
        }
        if (!MK_RBUTTON)
        {
            Mouse.RightDown = true;
        }
        else
        {
            Mouse.RightDown = false;
        }
    }break;
    default:
    {
        Ret = DefWindowProcA(handle, message, wParam, lParam);
    }
   case WM_SIZE:
    {
        static bool Trigger = false;
        if (Trigger)
        {
            GFX->OnWindowResize(Handle.get());
        }
    }break;
    }
    return Ret;
}

LRESULT Window::Adapter(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    //CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    Window* HandleWindow = reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA));
    return HandleWindow->WindowProc(handle, message, wParam, lParam);
}


