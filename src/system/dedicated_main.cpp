#include "arguments.hpp"
#include "getargs/argument_parser.hpp"
#include "common/labels.hpp"
#include "generator/makefile_generation.hpp"
#include "makefile/data_types.hpp"

#include <filesystem>
#include <fstream>

constexpr const char* ERR_STR_SOURCE_DIR_INVALID = "The provided source directory is invalid/doesn't exist!";

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

    if(Flags::UpdateSourceDirs.IsActive() && makefile_already_exists)
    {
        SafeReturn replacement_string = try_GetSourceDirectories();

        // FIXME: Repeated code
        if(replacement_string.ErrorCode() == Err::Generator::SOURCE_DIR_INVALID)
        {
            printf("%s%s\n%s", ERROR(), ERR_STR_SOURCE_DIR_INVALID, COLOR_RESET);
            return 1;
        }

        std::fstream makefile;
        std::stringstream makefile_string_stream;
        std::string makefile_string;

        makefile.open("Makefile");
        makefile_string_stream << makefile.rdbuf();
        makefile_string = makefile_string_stream.str();

        size_t erase_start = makefile_string.find("SRC_DIRS :=");
        size_t erase_end = makefile_string.rfind("\\\n", makefile_string.find("CC_SRCS")) + 3;

        makefile_string.erase(erase_start, (erase_end - erase_start));
        makefile_string.insert(erase_start, replacement_string.Data().GetLine());

        makefile.seekp(0);
        makefile << makefile_string;
        makefile.close();
        return 0;
    }

    SafeReturn try_GetMakefile = try_GenerateDefaultMakefile();
    ErrCode error_code = try_GetMakefile.ErrorCode();

    // FIXME: Repeated code
    if(error_code == Err::Generator::SOURCE_DIR_INVALID)
    {
        printf("%s%s\n%s", ERROR(), ERR_STR_SOURCE_DIR_INVALID, COLOR_RESET);
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
