#ifndef ARGUMENTS_HANDLER_H
#define ARGUMENTS_HANDLER_H

#include "argument.hpp"
#include <map>

class ArgumentsHandler
{
public:
    static void AddOption(const _Arg& Option);
    static void AddOption(const char* LongOption, const char* ShortOption);
    static void AddFlag(const _Arg& Flag);
    static void AddFlag(const char* LongFlag, const char* ShortFlag);

    static void ParseArguments(int NumberOfArguments, char** Arguments);

    static bool FlagActive(const _Arg& Flag);
    static bool OptionActive(const _Arg& Option);
    static const char* OptionValue(const _Arg& Option);

private:
    typedef const char* _value;
    typedef bool _status;
    static std::map<_Arg, _value> _options;
    static std::map<_Arg, _status> _flags;
};

extern ArgumentsHandler* global_ArgumentsHandler;

#endif // ARGUMENTS_HANDLER_H
