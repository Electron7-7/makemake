#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "common/safe_return.hpp"

#include <string>
#include <list>
#include <set>

struct OrganizedLibraries
{
    std::list<std::string> linux_library_files = {};
    std::list<std::string> linux_library_subdirs = {};
    std::list<std::string> windows_library_files = {};
    std::list<std::string> windows_library_subdirs = {};

    void RemoveDuplicates();
};

SafeReturn<std::list<std::string>> try_RecursiveGetDirectoriesContainingFileTypes(const std::string& StartingDirectory, std::set<std::string> FileExtensions);
SafeReturn<OrganizedLibraries> try_RecursiveFindLibraries();

#endif // FILESYSTEM_H
