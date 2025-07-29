#include "arguments.hpp"
#include "getargs/argument_parser.hpp"
#include "common/labels.hpp"
#include "generator/makefile_generator.hpp"
#include <filesystem>
#include <fstream>

std::string SourceCodeDirectory = "src";
std::string ProgramName = std::filesystem::current_path().stem().c_str();

int main(int argc, char** argv)
{
    // Add valid flags
    global_ArgumentParser->AddFlag(&Flags::Help);
    global_ArgumentParser->AddFlag(&Flags::Version);
    global_ArgumentParser->AddFlag(&Flags::DryRun);
    global_ArgumentParser->AddFlag(&Flags::debug_NoPrintout);

    // Add valid options
    global_ArgumentParser->AddOption(&Options::SourceDirectory);
    global_ArgumentParser->AddOption(&Options::ProgramName);

    // Parse all arguments
    global_ArgumentParser->ParseArguments(argc, argv);

    if(Flags::Help.IsActive())
    {
        printf("%s\n    %s\n", _Help_Printout, _Version_Printout);
        return 0;
    }

    if(Flags::Version.IsActive())
    {
        printf("    %s\n", _Version_Printout);
        return 0;
    }

    if(Options::SourceDirectory.IsActive() && Options::SourceDirectory.HasValue())
        SourceCodeDirectory = Options::SourceDirectory.GetValue();

    if(Options::ProgramName.IsActive() && Options::ProgramName.HasValue())
        ProgramName = Options::ProgramName.GetValue();

    SafeReturn<const char*> try_GetMakefile = GenerateDefaultMakefile();
    ErrCode error_code = try_GetMakefile.ErrorCode();

    if(error_code != Err::NO_ERROR)
    {
        switch(error_code)
        {
        case Err::Generator::SOURCE_DIR_INVALID:
            printf("%s The source directory is invalid/doesn't exist!\n%s", ERROR(), COLOR_RESET);
            break;
        }

        return 1;
    }

    if(Flags::DryRun.IsActive())
    {
        if(!Flags::debug_NoPrintout.IsActive())
            printf("%s\n", try_GetMakefile.Data());
        return 0;
    }

    std::ofstream makefile("Makefile");
    if(makefile)
    {
        makefile << try_GetMakefile.Data();
        makefile.close();
    }
    else
    {
        printf("%sUnable to write to %s!\n", ERROR(), (std::filesystem::current_path().string() + "/Makefile").c_str());
        return 1;
    }

    return 0;
}
