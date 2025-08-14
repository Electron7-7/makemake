#include "arguments.hpp"
#include "getargs/argument_parser.hpp"
#include "common/printing.hpp"
#include "makefile/interfacing.hpp"

#include <print>
#include <filesystem>
#include <fstream>

constexpr const char* default_SourceCodeDirectory = "src";
constexpr const char* default_LibrariesDirectory  = "src/lib";
const std::string     default_ProgramName = std::filesystem::current_path().stem().string();

#ifndef DEBUGGING
const char* constant_MakefileFileName = "Makefile";
#else
const char* constant_MakefileFileName = "DebugMakefile.make";
#endif // DEBUGGING

int main(int argc, char** argv)
{
    // Add valid flags
    global_ArgumentParser->AddFlag(&Flags::Help);
    global_ArgumentParser->AddFlag(&Flags::Version);
    global_ArgumentParser->AddFlag(&Flags::DryRun);
    global_ArgumentParser->AddFlag(&Flags::UpdateSourceDirs);
    global_ArgumentParser->AddFlag(&Flags::debug_NoPrintout);

    // Add valid options
    global_ArgumentParser->AddOption(&Options::SourceDirectory);
    global_ArgumentParser->AddOption(&Options::ProgramName);
    global_ArgumentParser->AddOption(&Options::Libraries);

    // Parse all arguments
    global_ArgumentParser->ParseArguments(argc, argv);

    if(Flags::Help.IsActive())
    {
        std::print("{}\n    {}\n", _Help_Printout, _Version_Printout);
        return 0;
    }

    if(Flags::Version.IsActive())
    {
        std::print("    {}\n", _Version_Printout);
        return 0;
    }

    if(Flags::debug_NoPrintout.IsActive())
    { Flags::DryRun.Activate(); }

    if(!Options::SourceDirectory.HasValue())
    { Options::SourceDirectory.SetValue(default_SourceCodeDirectory); }

    if(!Options::ProgramName.HasValue())
    { Options::ProgramName.SetValue(default_ProgramName.c_str()); }

    if(Flags::UpdateSourceDirs.IsActive())
    {
        if(!try_SetSourceDirs())
        {
            PRINT_ERROR("Failed to find source directory '{}'.", Options::SourceDirectory.GetValue());
            return 1;
        }
        return(!try_UpdateSourceDirs()); // false == '0', true == '1'
    }

    if(!Options::Libraries.HasValue())
    { Options::Libraries.SetValue(default_LibrariesDirectory); }

    if(Options::Libraries.IsActive())
    {
        try_SetLinkerFlags();
        return(!try_UpdateLinkerFlags()); // false == '0', true == '1'
    }

    GenerateDefaultMakefile();

    if(Flags::DryRun.IsActive())
    {
        if(!Flags::debug_NoPrintout.IsActive())
        { std::print("{}\n", GetMakefileBuffer()); }
        return 0;
    }

    std::ofstream makefile(constant_MakefileFileName);
    if(makefile)
    {
        makefile << GetMakefileBuffer();
    }
    else
    {
        PRINT_ERROR("Unable to write to {}!", std::filesystem::absolute(constant_MakefileFileName).generic_string());
        return 1;
    }

    makefile.close(); // FIXME: I don't know if you also need to call 'close' when the filestream fails
    return 0;
}
