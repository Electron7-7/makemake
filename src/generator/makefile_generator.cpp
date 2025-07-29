#include "makefile_generator.hpp"
#include "system/arguments.hpp"
#include "makefile/default_makefile.hpp"
#include <filesystem>
#include <set>

std::string source_directories = "";
std::string pretty_source_dirs_equal_sign = "";
int number_of_indent_spaces = 4;

ErrCode GetSourceDirectories()
{
    std::set<std::string> valid_directories = {};
    std::string longest_string = "";
    std::string src_dirs_variable("SRC_DIRS :=");

    if(!std::filesystem::is_directory(SourceCodeDirectory))
        return Err::Generator::SOURCE_DIR_INVALID;

    for(const auto& entry : std::filesystem::recursive_directory_iterator(SourceCodeDirectory))
    {
        if(entry.is_regular_file() && (entry.path().extension() == ".cpp" || entry.path().extension() == ".c"))
        {
            std::string valid_directory = entry.path().relative_path().remove_filename().generic_string();

            std::string truncated_directory = valid_directory.substr(SourceCodeDirectory.size() + 1);
            std::string final_directory = "$(SRC)/" + truncated_directory.substr(0, truncated_directory.size() - 1);

            if(!valid_directory.compare(SourceCodeDirectory + "/"))
                final_directory.erase(final_directory.size() - 1);

            valid_directories.insert(final_directory);

            if(final_directory.size() > longest_string.size())
                longest_string = final_directory;
        }
    }

    int longest_directory_length = longest_string.size() + number_of_indent_spaces;

    if(longest_directory_length < src_dirs_variable.length())
        longest_directory_length = src_dirs_variable.length();

    pretty_source_dirs_equal_sign = std::string(" :=").append((longest_directory_length - src_dirs_variable.size()), ' ') + " \\\n";

    for(const std::string& directory : valid_directories)
    {
        std::string spaces = " ";
        size_t number_of_spaces = (longest_directory_length - number_of_indent_spaces) - directory.size();
        spaces.append(number_of_spaces, ' ');
        source_directories += "    " + directory + spaces + "\\\n";
    }

    return Err::NO_ERROR;
}

SafeReturn<const char*> GenerateDefaultMakefile()
{
    std::string makefile = "";

    makefile += MakeVariables::LINUX_CXX.GetLine();
    makefile += MakeVariables::LINUX_CC.GetLine();

    makefile += "\n";

    makefile += "ifneq ($(OS),Windows_NT)\n";

    makefile += "\t" + MakeVariables::WINDOWS_CXX_LINUX.GetLine();
    makefile += "\t" + MakeVariables::WINDOWS_CC_LINUX.GetLine();

    makefile += "else\n";

    makefile += "\t" + MakeVariables::WINDOWS_CXX_WINDOWS.GetLine();
    makefile += "\t" + MakeVariables::WINDOWS_CC_WINDOWS.GetLine();

    makefile += "endif\n";
    makefile += "\n";

    makefile += MakeVariables::FLAGS_DEBUG_COMMON.GetLine();
    makefile += MakeVariables::FLAGS_DEBUG_LINUX.GetLine();
    makefile += MakeVariables::FLAGS_DEBUG_WINDOWS.GetLine();
    makefile += MakeVariables::FLAGS_RELEASE_COMMON.GetLine();
    makefile += MakeVariables::FLAGS_RELEASE_WINDOWS.GetLine();
    makefile += MakeVariables::FLAGS_RELEASE_LINUX.GetLine();
    makefile += MakeVariables::FLAGS_CXX_COMMON.GetLine();
    makefile += MakeVariables::FLAGS_CC_COMMON.GetLine();
    makefile += MakeVariables::FLAGS_WINDOWS.GetLine();
    makefile += MakeVariables::FLAGS_LINUX.GetLine();
    makefile += MakeVariables::LDFLAGS_LINUX.GetLine();
    makefile += MakeVariables::LDFLAGS_WINDOWS.GetLine();

    makefile += "\n";

    makefile += MakeVariables::INCLUDE.GetLine();

    makefile += "\n";

    makefile += MakeVariables::DIR_ROOT.GetLine();
    makefile += MakeVariables::DIR_LINUX.GetLine();
    makefile += MakeVariables::DIR_WINDOWS.GetLine();
    makefile += MakeVariables::DIR_DEBUG.GetLine();
    makefile += MakeVariables::DIR_RELEASE.GetLine();
    makefile += MakeVariables::DIR_OBJS.GetLine();

    makefile += "\n";

    makefile += make_variable_t("NAME_BASE", ProgramName, " := ").GetLine();

    makefile += "\n";

    makefile += "# LINUX\n";

    makefile += "ifneq ($(OS),Windows_NT)\n";

    makefile += "\t" + MakeVariables::IfOnLinux::EXPORT_NAME.GetLine();
    makefile += "\t" + MakeVariables::IfOnLinux::EXPORT_BUILD_ARCH.GetLine();
    makefile += "\t" + MakeVariables::IfOnLinux::EXPORT_DEBUG_FLAGS.GetLine();
    makefile += "\t" + MakeVariables::IfOnLinux::EXPORT_RELEASE_FLAGS.GetLine();
    makefile += "\t" + MakeVariables::IfOnLinux::EXPORT_CXX_FLAGS.GetLine();
    makefile += "\t" + MakeVariables::IfOnLinux::EXPORT_CC_FLAGS.GetLine();
    makefile += "\t" + MakeVariables::IfOnLinux::EXPORT_LD_FLAGS.GetLine();
    makefile += "\t" + MakeVariables::IfOnLinux::EXPORT_CXX.GetLine();
    makefile += "\t" + MakeVariables::IfOnLinux::EXPORT_CC.GetLine();

    makefile += "else # WINDOWS\n";

    makefile += "\t" + MakeVariables::IfOnWindows::EXPORT_NAME.GetLine();
    makefile += "\t" + MakeVariables::IfOnWindows::EXPORT_BUILD_ARCH.GetLine();
    makefile += "\t" + MakeVariables::IfOnWindows::EXPORT_DEBUG_FLAGS.GetLine();
    makefile += "\t" + MakeVariables::IfOnWindows::EXPORT_RELEASE_FLAGS.GetLine();
    makefile += "\t" + MakeVariables::IfOnWindows::EXPORT_CXX_FLAGS.GetLine();
    makefile += "\t" + MakeVariables::IfOnWindows::EXPORT_CC_FLAGS.GetLine();
    makefile += "\t" + MakeVariables::IfOnWindows::EXPORT_LD_FLAGS.GetLine();
    makefile += "\t" + MakeVariables::IfOnWindows::EXPORT_CXX.GetLine();
    makefile += "\t" + MakeVariables::IfOnWindows::EXPORT_CC.GetLine();

    makefile += "endif\n";
    makefile += "\n";

    makefile += MakeVariables::EXPORT_BUILD_VERSION.GetLine();
    makefile += MakeVariables::EXPORT_VERSION_FLAGS.GetLine();

    makefile += "\n";

    makefile += MakeVariables::EXPORT_BUILD_DIR.GetLine();
    makefile += MakeVariables::EXPORT_BUILD_OBJS.GetLine();

    makefile += "\n";

    makefile += MakeVariables::VPATH.GetLine();

    makefile += "\n";

    makefile += make_variable_t("SRC", SourceCodeDirectory.c_str(), " := ").GetLine();

    makefile += "\n";

    ErrCode global_SourceCodeDirectory_return_value = GetSourceDirectories();
    if(global_SourceCodeDirectory_return_value != Err::NO_ERROR)
        return SafeReturn<const char*>("", global_SourceCodeDirectory_return_value);

    makefile += make_variable_t("SRC_DIRS", source_directories.c_str(), pretty_source_dirs_equal_sign.c_str()).GetLine();

    makefile += "\n";

    makefile += MakeVariables::CC_SRCS.GetLine();
    makefile += MakeVariables::CXX_SRCS.GetLine();

    makefile += "\n";

    makefile += MakeVariables::EXPORT_CC_OBJS.GetLine();
    makefile += MakeVariables::EXPORT_CXX_OBJS.GetLine();

    makefile += "\n";

    makefile += MakeVariables::EXPORT_RESET.GetLine();
    makefile += MakeVariables::EXPORT_BLACK.GetLine();
    makefile += MakeVariables::EXPORT_RED.GetLine();
    makefile += MakeVariables::EXPORT_GREEN.GetLine();
    makefile += MakeVariables::EXPORT_YELLOW.GetLine();
    makefile += MakeVariables::EXPORT_BLUE.GetLine();
    makefile += MakeVariables::EXPORT_MAGENTA.GetLine();
    makefile += MakeVariables::EXPORT_CYAN.GetLine();
    makefile += MakeVariables::EXPORT_WHITE.GetLine();
    makefile += MakeVariables::EXPORT_DEFAULT.GetLine();

    makefile += "\n";

    makefile += MakeVariables::PHONY.GetLine();

    makefile += "\n";

    makefile += MakeTargets::TARGET_BUILD.GetLines();
    makefile += MakeTargets::TARGET_LINUX.GetLines();
    makefile += MakeTargets::TARGET_WINDOWS.GetLines();
    makefile += MakeTargets::TARGET_RELEASE.GetLines();
    makefile += MakeTargets::TARGET_DEBUG.GetLines();
    makefile += MakeTargets::TARGET_BUILD_DIR.GetLines();
    makefile += MakeTargets::TARGET_CLEAN.GetLines();
    makefile += MakeTargets::TARGET_DISABLE_COLORS.GetLines();
    makefile += MakeTargets::TARGET_CXX_OBJS.GetLines();
    makefile += MakeTargets::TARGET_CC_OBJS.GetLines();
    makefile += MakeTargets::TARGET_PROGRAM.GetLines();

    return SafeReturn(makefile.c_str());
}
