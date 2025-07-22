#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "argument.hpp"

namespace Flags
{
    const Flag Help    ("--help", "-h");
    const Flag Version ("--version", "-v");
}

namespace Options
{
    const Option DebugPrint       ("--debug-print", "-d");
    const Option GenerateMakefile ("--makefile", "-m");
}

constexpr const char* _Help_Printout =
R"~(    Usage: cpppp [-hv]

        -h, --help      print help document
        -v, --version   print program version
)~";

constexpr const char* _Version_Printout = "cpppp v0.0.1";

#endif // ARGUMENTS_H
