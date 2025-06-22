#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "argument.hpp"
#include <string>

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

inline std::string _Help_Printout =
R"~(
    Usage: cpppp [-hv]

        -h, --help      print help document
        -v, --version   print program version
)~";

inline std::string _Version_Printout = "0.0.1";

#endif // ARGUMENTS_H
