#include "arguments.hpp"
#include "getargs/argument_parser.hpp"
#include "common/printing.hpp"
#include "makefile/default_makefile.hpp"
#include "makefile/generating.hpp"
#include "makefile/editing.hpp"

#include <print>
#include <filesystem>
#include <fstream>
#include <array>
#include <list>

constexpr const char* ERR_STR_SOURCE_DIR_INVALID = "The provided source directory is invalid/doesn't exist!";
constexpr const char* default_SourceCodeDirectory = "src";
const std::string     default_ProgramName = std::filesystem::current_path().stem().string();

struct FileDir
{
    std::string filename = "";
    std::string directory = "";
};

std::array<std::string, 3> strings_Windows =
{
    "windows",
    "Windows",
    "WINDOWS",
};

std::array<std::string, 3> strings_Linux =
{
    "linux",
    "Linux",
    "LINUX",
};

enum class DirStatus
{
    LINUX    = 0b00,
    WINDOWS  = 0b01,
    AGNOSTIC = 0b10,
};

DirStatus CheckLibraryDir(const std::string& directory)
{
    for(std::string& string : strings_Windows)
    { if(directory.contains(string)) { return DirStatus::WINDOWS; } }

    for(std::string& string : strings_Linux)
        { if(directory.contains(string)) { return DirStatus::LINUX; } }

    return DirStatus::AGNOSTIC;
}

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

    if(!Options::SourceDirectory.IsActive() || !Options::SourceDirectory.HasValue())
    { Options::SourceDirectory.SetValue(default_SourceCodeDirectory); }

    if(!Options::ProgramName.IsActive() || !Options::ProgramName.HasValue())
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

    if(Options::Libraries.IsActive() && Options::Libraries.HasValue())
    {
        const char* library_path = Options::Libraries.GetValue();
        if(!std::filesystem::exists(std::filesystem::path(library_path)))
        { PRINT_WARNING("Specified library directory '{}' doesn't exist. The defeault Makefile will still be generated.", library_path) }
        else
        {
            std::list<std::string> linux_libraries = {};
            std::list<std::string> windows_libraries = {};

            std::list<std::string> linux_lib_subdirs = {};
            std::list<std::string> windows_lib_subdirs = {};

            std::string current_library = "";
            std::string current_lib_subdir = Options::Libraries.GetValue();

            std::filesystem::path library_path = current_lib_subdir;
            DirStatus library_subdir_status = DirStatus::AGNOSTIC;

            for(const auto& entry : std::filesystem::recursive_directory_iterator(library_path))
            {
                std::string path_name = entry.path().generic_string();

                if(entry.is_directory())
                {
                    library_subdir_status = CheckLibraryDir(path_name);
                    current_lib_subdir = path_name;
                }

                if(entry.is_regular_file())
                {
                    if(entry.path().parent_path() == library_path)
                    {
                        library_subdir_status = DirStatus::AGNOSTIC;
                        current_lib_subdir = library_path.generic_string();
                    }

                    current_library = entry.path().stem().generic_string().erase(0, 3); // library name minus the extension and "lib" (so, "libgetargs.a" becomes "getargs")

                    // FIXME: There's got to be a better way of doing this...
                    switch(library_subdir_status)
                    {
                    case DirStatus::AGNOSTIC:
                        break;
                    case DirStatus::WINDOWS:
                        windows_libraries.push_back(current_library);
                        windows_lib_subdirs.push_back(current_lib_subdir);
                        continue;
                    case DirStatus::LINUX:
                        linux_libraries.push_back(current_library);
                        linux_lib_subdirs.push_back(current_lib_subdir);
                        continue;
                    }

                    if(!entry.path().extension().generic_string().compare(".dll")) // Just in case
                    {
                        windows_libraries.push_back(current_library);
                        windows_lib_subdirs.push_back(current_lib_subdir);
                        continue;
                    }

                    windows_libraries.push_back(current_library);
                    windows_lib_subdirs.push_back(current_lib_subdir);
                    linux_libraries.push_back(current_library);
                    linux_lib_subdirs.push_back(current_lib_subdir);
                }
            }

            if(!linux_libraries.empty())
            {
                std::string new_value = MakeVariables::LDFLAGS_LINUX.ClearValue();

                for(const std::string& directory: linux_lib_subdirs)
                { new_value.append(" -L" + directory); }

                for(const std::string& library: linux_libraries)
                { new_value.append(" -l" + library); }

                new_value.pop_back();

                MakeVariables::LDFLAGS_LINUX.SetValue(new_value);
                PRINT_DEBUG("LDFLAGS_LINUX: '{}'", new_value)
            }

            if(!windows_libraries.empty())
            {
                std::string new_value = MakeVariables::LDFLAGS_WINDOWS.ClearValue();

                for(const std::string& directory: windows_lib_subdirs)
                { new_value.append(" -L" + directory); }

                for(const std::string& library: windows_libraries)
                { new_value.append(" -l" + library); }

                MakeVariables::LDFLAGS_WINDOWS.SetValue(new_value);
                PRINT_DEBUG("LDFLAGS_WINDOWS: '{}'", new_value)
            }
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
