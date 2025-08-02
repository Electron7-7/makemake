#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "argument.hpp"

namespace Flags
{
    const Flag Help     ("--help", "-h");
    const Flag Version  ("--version", "-v");
    const Flag DryRun   ("--dry-run", "-n");

    const Flag debug_NoPrintout ("--no-printout", "");
}

namespace Options
{
    const Option SourceDirectory ("--source", "-s");
    const Option ProgramName     ("--name", "-p");
}

constexpr const char* _Help_Printout =

R"~(    Usage: makemake [-hvn] [--no-color] [-s|--source <source_code_dir>] [-p|--name <global_ProgramName>]
        -h, --help              print help document
        -v, --version           print program version
        -n, --dry-run           print the generated Makefile instead of creating/replacing it
        -s, --source SOURCEDIR  set the source code directory (defaults to 'src')
        -p, --name NAME         set the name of the final binary (defaults to the name of the current directory)
)~";

constexpr const char* _Version_Printout = "makemake v1.0.2";

inline std::string global_SourceCodeDirectory = "src";
inline std::string global_ProgramName = "";
inline bool global_DryRun = false;
inline bool _global_DebugNoPrintOut = false;

#endif // ARGUMENTS_H
