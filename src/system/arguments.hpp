#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "getargs/argument.hpp"

namespace Flags
{
    inline Flag Help             ("--help", "-h");
    inline Flag Version          ("--version", "-v");
    inline Flag DryRun           ("--dry-run", "-n");
    inline Flag NoColor          ("--no-color");
    inline Flag UpdateSourceDirs ("--update-source-dirs", "-u");

    inline Flag debug_NoPrintout ("--no-printout");
}

namespace Options
{
    inline Option SourceDirectory ("--source", "-s");
    inline Option ProgramName     ("--name", "-p");
    inline Option Libraries       ("--libraries", "-l");
}

constexpr const char* _Help_Printout =
R"~(    Usage: makemake [-h|--help] [-v|--version] [-n|--dry-run] [--no-color]
                    [-s|--source <directory>] [-p|--name <name>]
        -h, --help                  print help document
        -v, --version               print program version
        -n, --dry-run               print the generated Makefile instead of creating/replacing it
        -s, --source <directory>    specify the root directory for the source code (defaults to './src')
        -u, --update-source-dirs    only update the source directories in an existing Makefile (generates a new Makefile if none exists)
        -p, --name <name>           set the name of the final binary (defaults to the name of the current directory)
)~";

constexpr const char* _Version_Printout = "makemake v2.1.2";

#endif // ARGUMENTS_H
