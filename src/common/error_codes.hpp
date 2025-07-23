#ifndef ERROR_CODES_H
#define ERROR_CODES_H

typedef unsigned short ErrCode;

namespace Err
{
    constexpr unsigned short NO_ERROR = 0x0;

    namespace Args
    {
        constexpr unsigned short MANDATORY_OPTION_MISSING = 0x1; // i.e: a missing flag or command line option
        constexpr unsigned short MANDATORY_INPUT_MISSING  = 0x2; // i.e: a missing input file
    }

    namespace Generator
    {
        constexpr unsigned short SOURCE_DIR_INVALID = 0x3;
    }
}

template<typename T>
struct SafeReturn
{
public:
    SafeReturn(T Data, ErrCode ErrorCode = Err::NO_ERROR)
    : data(Data), error_code(ErrorCode)
    {}

    ErrCode ErrorCode() const { return error_code; }
    T Data() const { return data; }

private:
    T data;
    ErrCode error_code = Err::NO_ERROR;
};

#endif // ERROR_CODES_H
