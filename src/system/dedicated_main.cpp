#include "common/platform.hpp"
#include "arguments/arguments.hpp"
#include "arguments/arguments_parser.hpp"
#include "argument_handlers.hpp"

int DedicatedMain(int argc, char** argv)
{
    // Add valid flags
    global_ArgumentsParser->AddFlag(Flags::Help);
    global_ArgumentsParser->AddFlag(Flags::Version);

    // Add valid options
    global_ArgumentsParser->AddOption(Options::DebugPrint);

    // Parse all arguments
    global_ArgumentsParser->ParseArguments(argc, argv);

    // Handle flags
    if(unsigned short return_value = FlagsHandler(global_ArgumentsParser->GetFlags()) != Err::NO_ERROR)
        return return_value;

    // Handle options
    if(unsigned short return_value = OptionsHandler(global_ArgumentsParser->GetOptions()) != Err::NO_ERROR)
        return return_value;

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
int main(int argc, char** argv)
{
    int linux_return = DedicatedMain(argc, argv);
    return linux_return;
}
#endif // PLAT_LINUX
