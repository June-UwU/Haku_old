#include <windows.h>
#include "Window.h"




int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Window window;
    
    // Run the message loop.
       
    while (window.HandleMessages())
    {
    }

    return 0;
}
