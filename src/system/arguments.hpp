#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "getargs/argument.hpp"

namespace Flags
{
    inline Flag Help    ("--help"    , "-h");
    inline Flag Version ("--version" , "-v");
    inline Flag DryRun  ("--dry-run" , "-n");
    inline Flag NoColor ("--no-color"      );
    inline Flag Update  ("--update"  , "-u");

    inline Flag debug_NoPrintout ("--no-printouts");
}

namespace Options
{
    inline Option SourceDirectory ("--source"   , "-s");
    inline Option ProgramName     ("--name"     , "-p");
    inline Option Libraries       ("--libraries", "-l");
}

constexpr const char* _Help_Printout =
R"~(    Usage: makemake [-h|--help] [-v|--version] [-n|--dry-run] [--no-color]
                    [-s|--source <directory>] [-l|--libraries <directory>] [-p|--name <name>]
        -h, --help                      print help document
        -v, --version                   print program version
        -n, --dry-run                   print finished output instead of writing it to the Makefile
        -u, --update                    used with -s, -l, -p, etc; updates an existing Makefile, only changing user-specified variables/targets
        -s, --source <directory>        specify the top-level source code directory (defaults to './src')
        -l, --libraries <directory>     specify the top-level library directory, e.g: 'src/lib' (no default)
        -p, --name <name>               set the name of the final binary (defaults to the name of the current directory)
)~";

constexpr const char* _Version_Printout = "makemake v2.3.3";

#endif // ARGUMENTS_H
