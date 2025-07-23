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
    const Option SourceDirectory ("--source", "-s");
    const Option ProgramName     ("--name", "-n");
}

constexpr const char* _Help_Printout =
R"~(    Usage: cpppp [-hv]

        -h, --help      print help document
        -v, --version   print program version
)~";

constexpr const char* _Version_Printout = "cpppp v0.0.1";

inline std::string source_directory = "src";
inline std::string program_name = "";

#endif // ARGUMENTS_H
