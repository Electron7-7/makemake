#include "arguments/arguments.hpp"
#include "arguments/arguments_parser.hpp"
#include "argument_handlers.hpp"
#include "common/labels.hpp"
#include "generator/makefile_generator.hpp"
#include <filesystem>
#include <fstream>

int main(int argc, char** argv)
{
    // Add valid flags
    global_ArgumentsParser->AddFlag(Flags::Help);
    global_ArgumentsParser->AddFlag(Flags::Version);
    global_ArgumentsParser->AddFlag(Flags::DryRun);
    global_ArgumentsParser->AddFlag(Flags::debug_NoPrintout);

    // Add valid options
    global_ArgumentsParser->AddOption(Options::SourceDirectory);
    global_ArgumentsParser->AddOption(Options::ProgramName);

    // Parse all arguments
    global_ArgumentsParser->ParseArguments(argc, argv);

    // Handle flags
    if(unsigned short return_value = FlagsHandler(global_ArgumentsParser->GetFlags()) != Err::NO_ERROR)
        return return_value;

    // Handle options
    if(unsigned short return_value = OptionsHandler(global_ArgumentsParser->GetOptions()) != Err::NO_ERROR)
        return return_value;

    SafeReturn<std::string> try_GetMakefile = GenerateDefaultMakefile();
    ErrCode error_code = try_GetMakefile.ErrorCode();

    if(error_code != Err::NO_ERROR)
    {
        switch(error_code)
        {
        case Err::Generator::SOURCE_DIR_INVALID:
            printf("%s The source directory is invalid/doesn't exist!\n%s", ERROR, COLOR_RESET);
            break;
        }

        return 1;
    }

    if(global_DryRun)
    {
        if(!_global_DebugNoPrintOut)
            printf("%s\n", try_GetMakefile.Data().c_str());
        return 0;
    }

    std::ofstream makefile("Makefile"); // FIXME: Check if file is able to be written to!
    if(makefile)
    {
        makefile << try_GetMakefile.Data();
        makefile.close();
    }
    else
    {
        printf("%sUnable to write to %s!\n", ERROR, (std::filesystem::current_path().string() + "/Makefile").c_str());
        return 1;
    }

    return 0;
}
