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

const char* constant_MakefileFileName = "Makefile";

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

    if(Flags::UpdateSourceDirs.IsActive() && std::filesystem::exists(std::filesystem::path("Makefile")))
    {
        if(!try_UpdateSourceDirectories())
        {
            // FIXME: Repeated code
            PRINT_ERROR("Failed to update source directory. {}", ERR_STR_SOURCE_DIR_INVALID);
            return 1;
        }
        return 0;
    }
            }

    if(!Options::Libraries.HasValue())
    { Options::Libraries.SetValue(default_LibrariesDirectory); }

            {
        }
    }

    SafeReturn try_GetMakefile = try_GenerateDefaultMakefile();
    ErrCode error_code = try_GetMakefile.ErrorCode();

    if(error_code == Err::Generating::SOURCE_DIR_INVALID)
    {
        // FIXME: Repeated code
        PRINT_ERROR("Failed to generate Makefile. {}", ERR_STR_SOURCE_DIR_INVALID);
        return 1;
    }

    if(Flags::DryRun.IsActive())
    {
        if(!Flags::debug_NoPrintout.IsActive())
        { std::print("{}\n", try_GetMakefile.Data()); }
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
        PRINT_ERROR("Unable to write to {}!", (std::filesystem::current_path().string() + "/Makefile"));
        return 1;
    }

    return 0;
}
