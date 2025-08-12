#include "arguments.hpp"
#include "getargs/argument_parser.hpp"
#include "common/labels.hpp"
#include "makefile/generating.hpp"
#include "makefile/editing.hpp"

#include <print>
#include <filesystem>
#include <fstream>

const char* ERR_STR_SOURCE_DIR_INVALID = "The provided source directory is invalid/doesn't exist!";

std::string SourceCodeDirectory = "src";
std::string ProgramName = std::filesystem::current_path().stem().c_str();
bool makefile_already_exists = std::filesystem::exists(std::filesystem::path("Makefile"));

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

    if(Options::SourceDirectory.IsActive() && Options::SourceDirectory.HasValue())
        SourceCodeDirectory = Options::SourceDirectory.GetValue();

    if(Options::ProgramName.IsActive() && Options::ProgramName.HasValue())
        ProgramName = Options::ProgramName.GetValue();

    if(Flags::UpdateSourceDirs.IsActive() && makefile_already_exists)
    {
        if(!try_UpdateSourceDirectories())
        {
            // FIXME: Repeated code
            std::print("{}{}\n{}", ERROR(), ERR_STR_SOURCE_DIR_INVALID, COLOR_RESET);
            return 1;
        }
        return 0;
    }

    SafeReturn try_GetMakefile = try_GenerateDefaultMakefile();
    ErrCode error_code = try_GetMakefile.ErrorCode();

    if(error_code == Err::Generating::SOURCE_DIR_INVALID)
    {
        // FIXME: Repeated code
        std::print("{}{}\n{}", ERROR(), ERR_STR_SOURCE_DIR_INVALID, COLOR_RESET);
        return 1;
    }

    if(Flags::DryRun.IsActive())
    {
        if(!Flags::debug_NoPrintout.IsActive())
            std::print("{}\n", try_GetMakefile.Data());
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
        std::print("{}Unable to write to {}!\n", ERROR(), (std::filesystem::current_path().string() + "/Makefile").c_str());
        return 1;
    }

    return 0;
}
