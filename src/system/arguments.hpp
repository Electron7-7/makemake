#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "getargs/argument.hpp"

#include <string>

namespace Flags
{
    static Flag Help             ("--help", "-h");
    static Flag Version          ("--version", "-v");
    static Flag DryRun           ("--dry-run", "-n");
    static Flag NoColor          ("--no-color");
    static Flag UpdateSourceDirs ("--update-source-dirs", "-u");

    static Flag debug_NoPrintout("--no-printout");
}

namespace Options
{
    static Option SourceDirectory ("--source", "-s");
    static Option ProgramName     ("--name", "-p");
}

constexpr const char* _Help_Printout =

R"~(    Usage: makemake [-hvn] [--no-color] [-s|--source <source_code_dir>] [-p|--name <global_ProgramName>]
        -h, --help                  print help document
        -v, --version               print program version
        -n, --dry-run               print the generated Makefile instead of creating/replacing it
        -s, --source SOURCEDIR      set the source code directory (defaults to 'src')
        -u, --update-source-dirs    only update the source directories in an existing Makefile (generates a new Makefile if none exists)
        -p, --name NAME             set the name of the final binary (defaults to the name of the current directory)
)~";

constexpr const char* _Version_Printout = "makemake v2.1.1";

extern std::string SourceCodeDirectory;
extern std::string ProgramName;
#endif // ARGUMENTS_H
