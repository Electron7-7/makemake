#include "interfacing.hpp"
#include "template.hpp"
#include "system/arguments.hpp"
#include "system/filesystem.hpp"
#include "common/printing.hpp"
#include "common/colors.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>

namespace FileSys = std::filesystem; // Look, I'm lazy and 'std::filesystem' takes up so much fucking space to type out

using namespace MakeVariables;
using namespace MakeTargets;

constexpr unsigned short number_of_indent_spaces = 4;

// Using hardcoded file extensions for now (I may expand `makemake` to support other languages/custom extensions)
constexpr const char* CPP = ".cpp";
constexpr const char* C = ".c";

static std::string makefile_buffer = "";

bool CheckMakefileExists()
{
    FileSys::path absolute_makefile(FileSys::absolute(constant_MakefileFileName)); // Hehe, absolute cinema
    if(!FileSys::exists(absolute_makefile))
    {
        PRINT_ERROR("Cannot update '{}' as it does not exist.", absolute_makefile.generic_string())
        return false;
    }
    return true;
}

bool try_SetSourceDirs()
{
    SRC.SetValue(Options::SourceDirectory.GetValue());

    SafeReturn directories_list = try_RecursiveGetDirectoriesContainingFileTypes(SRC.GetValue(), {CPP, C});

    if(directories_list.Status() != Status::NO_ERROR)
    { return false; }

    std::list<std::string> directories = directories_list.Data();

    size_t first_line_length = (SRC_DIRS.GetName() + SRC_DIRS.GetSign()).size();
    std::string longest_directory("");

    for(std::string& directory : directories)
    {
        directory = "$(SRC)/" + directory.substr(SRC.GetValue().size() + 1);

        if(directory.back() == '/') { directory.pop_back(); }

        if(directory.size() > longest_directory.size())
        { longest_directory = directory; }
    }

    int longest_directory_length = longest_directory.size() + number_of_indent_spaces;

    if(longest_directory_length < first_line_length)
    { longest_directory_length = first_line_length; }

    SRC_DIRS.SetSign(SRC_DIRS.GetSign().append(longest_directory_length - first_line_length, ' ') + " \\\n");

    std::string src_dir_new_value("");

    for(const std::string& directory : directories)
    {
        std::string spaces(" ");
        size_t number_of_spaces = (longest_directory_length - number_of_indent_spaces) - directory.size();
        spaces.append(number_of_spaces, ' ');
        src_dir_new_value.append(number_of_indent_spaces, ' ');
        src_dir_new_value += (directory + spaces + "\\\n");
    }

    SRC_DIRS.SetValue(src_dir_new_value);
    return true;
}

bool try_SetLinkerFlags()
{
    SafeReturn<OrganizedLibraries> try_get_libraries = try_RecursiveFindLibraries();

    if(try_get_libraries.Status() != Status::NO_ERROR)
    {
        if(Options::Libraries.IsActive()) { PRINT_WARNING("Unable to find valid libraries at '{}'", Options::Libraries.GetValue()) }
        return false;
    }

    const OrganizedLibraries& libraries = try_get_libraries.Data();

    std::string linux_ldflags = LDFLAGS_LINUX.ClearValue();
    std::string windows_ldflags = LDFLAGS_WINDOWS.ClearValue();

    for(const std::string& directory : libraries.linux_library_subdirs)
    { linux_ldflags.append(" -L" + directory); }
    for(const std::string& library : libraries.linux_library_files)
    { linux_ldflags.append(" -l" + library); }

    for(const std::string& directory : libraries.windows_library_subdirs)
    { windows_ldflags.append(" -L" + directory); }
    for(const std::string& library : libraries.windows_library_files)
    { windows_ldflags.append(" -l" + library); }

    LDFLAGS_LINUX.SetValue(linux_ldflags);
    LDFLAGS_WINDOWS.SetValue(windows_ldflags);
    return true;
}

bool try_UpdateSourceDirs()
{ return !static_cast<bool>(try_ReplaceVariable(&SRC_DIRS, "$(SRC)/")); }

bool try_UpdateLinkerFlags()
{ return !static_cast<bool>(try_ReplaceVariable(&LDFLAGS_LINUX) | try_ReplaceVariable(&LDFLAGS_WINDOWS)); }

int try_ReplaceVariable(make_variable_t* variable, std::string multiline_check)
{
    if(!CheckMakefileExists()) { return 1; }

    std::fstream makefile;
    std::stringstream makefile_string_stream;
    std::string makefile_string;

    makefile.open(constant_MakefileFileName);
    makefile_string_stream << makefile.rdbuf();
    makefile_string = makefile_string_stream.str();
    makefile.close();

    std::string line_beginning( variable->GetName() + (variable->GetSign().substr(0, 3)) );

    size_t erase_start = makefile_string.find(line_beginning);
    size_t erase_end = erase_start;

    if(erase_start == std::string::npos || makefile_string.at(erase_start - 1) != '\n')
    { return 1; }

    size_t iterator = (erase_start - 1);
    size_t size_of_makefile = makefile_string.size();
    std::string line_buffer = "";
    char current_character = ' ';

    if(multiline_check.empty()) // Single-line variable process
    {
        PRINT_DEBUG("Processing a single-line variable")
        while(current_character != '\n' && ++iterator < size_of_makefile)
        {
            current_character = makefile_string.at(iterator);
            line_buffer += current_character;
        }

        erase_end = iterator;
    }
    else // Multi-line variable process
    {
        PRINT_DEBUG("Processing a multi-line variable")
        while(++iterator < size_of_makefile)
        {
            current_character = makefile_string.at(iterator);

            if(current_character != '\n')
            {
                line_buffer += current_character;
                continue;
            }

            if(line_buffer.starts_with(line_beginning) || line_buffer.contains(multiline_check))
            {
                line_buffer.clear();
                continue;
            }

            iterator -= line_buffer.size();
            break;
        }

        erase_end = (iterator - 1);
    }

    if(erase_end <= erase_start)
    {
        PRINT_ERROR("try_ReplaceVariable - 'erase_end <= erase_start' returned true! Potential issue: 'try_ReplaceVariable' was told to process a multiline variable, but only read one line.")
        return 1;
    }

    std::string trimmed_variable = variable->GetLine();
    std::string check_for_newlines((trimmed_variable.end() - 2).base());

    /*
        * Quick Description for why I use `pop_back()` 1 or 2 times:
        * The replacement string ends with two empty lines. The first one always gets
        * removed, but the second one only gets removed if the character right after
        * the "to be replaced" selection is also a new line. The way I've formatted
        * these directory entries adds a backslash to the end of the last entry, and
        * `make` will throw an error if there isn't an empty line immediately after.
    */

    if(check_for_newlines.at(1) == '\n')
    { trimmed_variable.pop_back(); }

    if(check_for_newlines.at(0) == '\n' && makefile_string.at(erase_end + 1) == '\n')
    { trimmed_variable.pop_back(); }

    PRINT_DEBUG("Erasing: '{}{}{}{}'", Color::Back::RedBold, Color::Front::WhiteBold, makefile_string.substr(erase_start, (erase_end - erase_start)), Color::Reset);
    PRINT_DEBUG("Inserting: '{}{}{}{}'", Color::Back::GreenBold, Color::Front::WhiteBold, trimmed_variable, Color::Reset);

    if(Flags::DryRun.IsActive())
    {
#ifndef DEBUGGING
        std::print("{}\0", trimmed_variable);
#endif
        return 0;
    }

    makefile_string.erase(erase_start, (erase_end - erase_start));
    makefile_string.insert(erase_start, trimmed_variable);

    std::filesystem::remove(std::filesystem::path(constant_MakefileFileName));

    makefile.open(constant_MakefileFileName, std::ios_base::out);
    makefile << makefile_string;
    makefile.close();

    return 0;
}

const char* GetMakefileBuffer()
{ return makefile_buffer.c_str(); }

void GenerateDefaultMakefile()
{
    makefile_buffer += LINUX_CXX.GetLine();
    makefile_buffer += LINUX_CC.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += "ifneq ($(OS),Windows_NT)\n";

    makefile_buffer += "\t" + WINDOWS_CXX_LINUX.GetLine();
    makefile_buffer += "\t" + WINDOWS_CC_LINUX.GetLine();

    makefile_buffer += "else\n";

    makefile_buffer += "\t" + WINDOWS_CXX_WINDOWS.GetLine();
    makefile_buffer += "\t" + WINDOWS_CC_WINDOWS.GetLine();

    makefile_buffer += "endif\n";
    makefile_buffer += "\n";

    makefile_buffer += FLAGS_DEBUG_COMMON.GetLine();
    makefile_buffer += FLAGS_DEBUG_LINUX.GetLine();
    makefile_buffer += FLAGS_DEBUG_WINDOWS.GetLine();
    makefile_buffer += FLAGS_RELEASE_COMMON.GetLine();
    makefile_buffer += FLAGS_RELEASE_WINDOWS.GetLine();
    makefile_buffer += FLAGS_RELEASE_LINUX.GetLine();
    makefile_buffer += FLAGS_CXX_COMMON.GetLine();
    makefile_buffer += FLAGS_CC_COMMON.GetLine();
    makefile_buffer += FLAGS_WINDOWS.GetLine();
    makefile_buffer += FLAGS_LINUX.GetLine();

    try_SetLinkerFlags();

    makefile_buffer += LDFLAGS_LINUX.GetLine();
    makefile_buffer += LDFLAGS_WINDOWS.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += INCLUDE.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += DIR_ROOT.GetLine();
    makefile_buffer += DIR_LINUX.GetLine();
    makefile_buffer += DIR_WINDOWS.GetLine();
    makefile_buffer += DIR_DEBUG.GetLine();
    makefile_buffer += DIR_RELEASE.GetLine();
    makefile_buffer += DIR_OBJS.GetLine();

    makefile_buffer += "\n";

    // Todo: Move this to a function(?)
    NAME_BASE.SetValue(Options::ProgramName.GetValue());
    makefile_buffer += NAME_BASE.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += "# LINUX\n";

    makefile_buffer += "ifneq ($(OS),Windows_NT)\n";

    makefile_buffer += "\t" + IfOnLinux::EXPORT_NAME.GetLine();
    makefile_buffer += "\t" + IfOnLinux::EXPORT_BUILD_ARCH.GetLine();
    makefile_buffer += "\t" + IfOnLinux::EXPORT_DEBUG_FLAGS.GetLine();
    makefile_buffer += "\t" + IfOnLinux::EXPORT_RELEASE_FLAGS.GetLine();
    makefile_buffer += "\t" + IfOnLinux::EXPORT_CXX_FLAGS.GetLine();
    makefile_buffer += "\t" + IfOnLinux::EXPORT_CC_FLAGS.GetLine();
    makefile_buffer += "\t" + IfOnLinux::EXPORT_LD_FLAGS.GetLine();
    makefile_buffer += "\t" + IfOnLinux::EXPORT_CXX.GetLine();
    makefile_buffer += "\t" + IfOnLinux::EXPORT_CC.GetLine();

    makefile_buffer += "else # WINDOWS\n";

    makefile_buffer += "\t" + IfOnWindows::EXPORT_NAME.GetLine();
    makefile_buffer += "\t" + IfOnWindows::EXPORT_BUILD_ARCH.GetLine();
    makefile_buffer += "\t" + IfOnWindows::EXPORT_DEBUG_FLAGS.GetLine();
    makefile_buffer += "\t" + IfOnWindows::EXPORT_RELEASE_FLAGS.GetLine();
    makefile_buffer += "\t" + IfOnWindows::EXPORT_CXX_FLAGS.GetLine();
    makefile_buffer += "\t" + IfOnWindows::EXPORT_CC_FLAGS.GetLine();
    makefile_buffer += "\t" + IfOnWindows::EXPORT_LD_FLAGS.GetLine();
    makefile_buffer += "\t" + IfOnWindows::EXPORT_CXX.GetLine();
    makefile_buffer += "\t" + IfOnWindows::EXPORT_CC.GetLine();

    makefile_buffer += "endif\n";
    makefile_buffer += "\n";

    makefile_buffer += EXPORT_BUILD_VERSION.GetLine();
    makefile_buffer += EXPORT_VERSION_FLAGS.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += EXPORT_BUILD_DIR.GetLine();
    makefile_buffer += EXPORT_BUILD_OBJS.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += EXPORT_CLEAN_ARCH.GetLine();
    makefile_buffer += EXPORT_CLEAN_VERSION.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += VPATH.GetLine();

    makefile_buffer += "\n";

    try_SetSourceDirs();

    makefile_buffer += SRC.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += SRC_DIRS.GetLine();

    makefile_buffer += CC_SRCS.GetLine();
    makefile_buffer += CXX_SRCS.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += EXPORT_CC_OBJS.GetLine();
    makefile_buffer += EXPORT_CXX_OBJS.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += EXPORT_RESET.GetLine();
    makefile_buffer += EXPORT_BLACK.GetLine();
    makefile_buffer += EXPORT_RED.GetLine();
    makefile_buffer += EXPORT_GREEN.GetLine();
    makefile_buffer += EXPORT_YELLOW.GetLine();
    makefile_buffer += EXPORT_BLUE.GetLine();
    makefile_buffer += EXPORT_MAGENTA.GetLine();
    makefile_buffer += EXPORT_CYAN.GetLine();
    makefile_buffer += EXPORT_WHITE.GetLine();
    makefile_buffer += EXPORT_DEFAULT.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += PHONY.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += TARGET_BUILD.GetLines();
    makefile_buffer += TARGET_LINUX.GetLines();
    makefile_buffer += TARGET_WINDOWS.GetLines();
    makefile_buffer += TARGET_RELEASE.GetLines();
    makefile_buffer += TARGET_DEBUG.GetLines();
    makefile_buffer += TARGET_BUILD_DIR.GetLines();

    makefile_buffer += CLEAN_FUNCTIONS_COMMENT.GetLine();
    makefile_buffer += CLEAN_FUNCTION.GetLine();
    makefile_buffer += CLEAN_OBJS_FUNCTION.GetLine();
    makefile_buffer += CLEAN_DIRTY_FUNCTION.GetLine();
    makefile_buffer += CLEAN_PRINT_FUNCTION.GetLine();

    makefile_buffer += "\n";

    makefile_buffer += CLEAN_TARGET_COMMENT.GetLine();
    makefile_buffer += TARGET_CLEAN_TARGET.GetLines();

    makefile_buffer += CLEAN_ALL_COMMENT.GetLine();
    makefile_buffer += TARGET_CLEAN_ALL.GetLines();

    makefile_buffer += CLEAN_COMMENT.GetLine();
    makefile_buffer += TARGET_CLEAN.GetLines();

    makefile_buffer += MOSTLY_CLEAN_COMMENT.GetLine();
    makefile_buffer += TARGET_MOSTLY_CLEAN.GetLines();

    makefile_buffer += TARGET_DISABLE_COLORS.GetLines();

    makefile_buffer += CXX_OBJS_COMMENT.GetLine();
    makefile_buffer += TARGET_CXX_OBJS.GetLines();

    makefile_buffer += C_OBJS_COMMENT.GetLine();
    makefile_buffer += TARGET_CC_OBJS.GetLines();

    makefile_buffer += PROGRAM_COMMENT.GetLine();
    makefile_buffer += TARGET_PROGRAM.GetLines();

    makefile_buffer += "\n";

    makefile_buffer += NOTHING_TO_CLEAN_COMMENT.GetLine();
    makefile_buffer += TARGET_NOTHING_TO_CLEAN.GetLines();

    makefile_buffer += CLEAN_PRINTOUT_COMMENT.GetLine();
    makefile_buffer += TARGET_CLEAN_PRINTOUT.GetLines();

    makefile_buffer.pop_back(); // Remove the extraneous newline
}
