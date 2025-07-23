#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "argument.hpp"

namespace Flags
{
    const Flag Help     ("--help", "-h");
    const Flag Version  ("--version", "-v");
    const Flag DryRun   ("--dry-run", "-n");
}

namespace Options
{
    const Option SourceDirectory ("--source", "-s");
    const Option ProgramName     ("--name", "-p");
}

constexpr const char* _Help_Printout =

R"~(    Usage: cpppp [-hvn] [--no-color] [-s|--source <source_code_dir>] [-p|--name <program_name>]
        -h, --help              print help document
        -v, --version           print program version
        -n, --dry-run           print the generated Makefile instead of creating/replacing it
        -s, --source SOURCEDIR  set the source code directory (defaults to 'src')
        -p, --name NAME         set the name of the final binary (defaults to the name of the current directory)
)~";

constexpr const char* _Version_Printout = "cpppp v0.0.1";

inline std::string source_directory = "src";
inline std::string program_name = "";
inline bool dry_run = false;

#endif // ARGUMENTS_H
