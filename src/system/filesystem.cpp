#include "filesystem.hpp"
#include "common/printing.hpp"

#include <filesystem>

using namespace std::filesystem;

typedef std::list<std::string> DirectoriesList;
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
