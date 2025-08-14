#include "filesystem.hpp"
#include "common/printing.hpp"

#include <filesystem>

using namespace std::filesystem;

typedef std::list<std::string> DirectoriesList;

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

void OrganizedLibraries::RemoveDuplicates()
{
    if(!linux_library_files.empty())
    {
        linux_library_files.sort();
        linux_library_files.unique();
    }
    if(!linux_library_subdirs.empty())
    {
        linux_library_subdirs.sort();
        linux_library_subdirs.unique();
    }
    if(!windows_library_files.empty())
    {
        windows_library_files.sort();
        windows_library_files.unique();
    }
    if(!windows_library_subdirs.empty())
    {
        windows_library_subdirs.sort();
        windows_library_subdirs.unique();
    }
}

SafeReturn<DirectoriesList> try_RecursiveGetDirectoriesContainingFileTypes(const std::string& start_directory, std::set<std::string> extensions)
{
    path start_path = start_directory;
    DirectoriesList valid_directories = {};

    if(!is_directory(start_path))
    {
        // PRINT_ERROR("try_RecursiveGetDirectoriesContainingFileTypes - Starting directory '{}' is invalid", start_directory)
        return SafeReturn(valid_directories, Status::INVALID_DIRECTORY);
    }

    for(const auto& entry : recursive_directory_iterator(start_path))
    {
        if(entry.is_regular_file() && extensions.contains(entry.path().extension().generic_string()))
        { valid_directories.push_back(entry.path().parent_path().generic_string()); }
    }

    valid_directories.sort();
    valid_directories.unique();

    return SafeReturn(valid_directories);
}

typedef unsigned char ArchType;
struct Architecture
{
    static constexpr ArchType LINUX    = 0b01;
    static constexpr ArchType WINDOWS  = 0b10;
    static constexpr ArchType AGNOSTIC = 0b11;
};

ArchType CheckLibraryDir(const std::string& directory)
{
    for(std::string& string : strings_Windows)
    { if(directory.contains(string)) { return Architecture::WINDOWS; } }

    for(std::string& string : strings_Linux)
    { if(directory.contains(string)) { return Architecture::LINUX; } }

    return Architecture::AGNOSTIC;
}

SafeReturn<OrganizedLibraries> try_RecursiveFindLibraries()
{
    std::string current_lib_file = "";
    std::string current_lib_subdir = Options::Libraries.GetValue();

    std::filesystem::path library_path = current_lib_subdir;
    ArchType library_subdir_status = Architecture::AGNOSTIC;

    OrganizedLibraries return_data = {};
    SafeStatus return_status = Status::NO_ERROR;

    if(!is_directory(current_lib_subdir))
    { return_status = Status::INVALID_DIRECTORY; }

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
                library_subdir_status = Architecture::AGNOSTIC;
                current_lib_subdir = library_path.generic_string();
            }

            current_lib_file = entry.path().stem().generic_string().erase(0, 3); // library name minus the extension and "lib" (so, "libgetargs.a" becomes "getargs")

            if(library_subdir_status & Architecture::LINUX)
            {
                return_data.linux_library_files.push_back(current_lib_file);
                return_data.linux_library_subdirs.push_back(current_lib_subdir);
            }

            if(library_subdir_status & Architecture::WINDOWS)
            {
                return_data.windows_library_files.push_back(current_lib_file);
                return_data.windows_library_subdirs.push_back(current_lib_subdir);
            }
        }
    }

    return_data.RemoveDuplicates();
    return SafeReturn<OrganizedLibraries>(return_data, return_status);
}
