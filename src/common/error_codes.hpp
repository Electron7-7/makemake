#ifndef ERROR_CODES_H
#define ERROR_CODES_H

typedef unsigned short ErrorCode;

namespace Err
{
    constexpr unsigned short NO_ERROR = 0x0;

    namespace Args
    {
        constexpr unsigned short MANDATORY_OPTION_MISSING = 0x1; // i.e: a missing flag or command line option
        constexpr unsigned short MANDATORY_INPUT_MISSING  = 0x2; // i.e: a missing input file
    }
}

#endif // ERROR_CODES_H
