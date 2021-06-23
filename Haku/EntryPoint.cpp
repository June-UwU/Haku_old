#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Application.h"
#include "Throwables.h"
#include "GFXThrowables.h"

/*SOMEBODY REMIND ME TO SAVE BEFORE EXITING JESUS*/

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    try
    {
        Application App;
        App.Run();
        return 0;
    } 
    catch (GFXThrowables& thrown)
    {
        MessageBoxA(nullptr, thrown.what(), "Haku-Chan Hiccups", MB_OK);
        std::exit(-1);
    }
    catch(Throwables &thrown)
    {
        MessageBoxA(nullptr, thrown.what(), "Haku-Chan Hiccups", MB_OK);
        std::exit(-1);
    }
    catch (std::exception& thrown)
    {
        MessageBoxA(nullptr, thrown.what(), "Haku-Chan Hiccups", MB_OK);
        std::exit(-1);
    }
}
