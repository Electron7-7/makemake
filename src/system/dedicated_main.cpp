#include "common/platform.hpp"
#include "common/debugging.hpp"

// DedicatedMain gets called by all platforms
int DedicatedMain(int argc, char* argv[])
{
    PRINT("Hello World!");
    return 0;
}

// System-specific main functions
#ifdef PLAT_WINDOWS
#include <windows.h>
#include <winbase.h>
#include <shellapi.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int argc,win_return = -1;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    win_return = DedicatedMain(argc, (char**)argv);
    GlobalFree(argv);
    return win_return;
}
#endif // PLAT_WINDOWS

#ifdef PLAT_APPLE
// Uhhhh, go fuck urself ig? Sorry...
#endif // PLAT_APPLE

#ifdef PLAT_LINUX
int main(int argc, char* argv[])
{
    int linux_return = DedicatedMain(argc, argv);
    return linux_return;
}
#endif // PLAT_LINUX
