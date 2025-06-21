#include "arguments_handler.hpp"
#include "common/debugging.hpp"
#include "common/compare_strings.hpp"
#include <cassert>

ArgumentsHandler static_ArgumentsHandler;
ArgumentsHandler* global_ArgumentsHandler = &static_ArgumentsHandler;

std::map<_Arg, const char*> ArgumentsHandler::_options = {};
std::map<_Arg, bool> ArgumentsHandler::_flags = {};

void ArgumentsHandler::AddOption(const _Arg& option)
{
    if(ArgumentsHandler::_options.contains(option))
        return;

    ArgumentsHandler::_options[option] = "";
}

void ArgumentsHandler::AddOption(const char* long_option, const char* short_option)
{ ArgumentsHandler::AddOption(_Arg(long_option, short_option)); }

void ArgumentsHandler::AddFlag(const _Arg& flag)
{
    if(ArgumentsHandler::_flags.contains(flag))
        return;

    ArgumentsHandler::_flags[flag] = false;
}

void ArgumentsHandler::AddFlag(const char* long_flag, const char* short_flag)
{ ArgumentsHandler::AddFlag(_Arg(long_flag, short_flag)); }

void ArgumentsHandler::ParseArguments(int argc, char** argv)
{
    if(argc < 1)
    {
        PRINTERR("ArgumentsHandler::ParseArguments - number of arguments < 1!");
        return;
    }

    for(unsigned int i = 1 ; i < argc ; i++)
    {
        if(ArgumentsHandler::_options.contains(argv[i]) && (i+1) < argc)
            ArgumentsHandler::_options.at(argv[i]) = argv[i+1];

        for(auto& [flag, value] : _flags)
            if(flag == argv[i])
                value = true;
    }
}

bool ArgumentsHandler::FlagActive(const _Arg& flag)
{ return(ArgumentsHandler::_flags.contains(flag) && ArgumentsHandler::_flags.at(flag)); }

bool ArgumentsHandler::OptionActive(const _Arg& option)
{ return(ArgumentsHandler::_options.contains(option) && !CompareStrings(ArgumentsHandler::_options.at(option), "")); }

const char* ArgumentsHandler::OptionValue(const _Arg& option)
{
    if(!ArgumentsHandler::_options.contains(option))
    {
        PRINTERR("ArgumentsHandler::OptionActive - option does not exist! Returning an empty string");
        return "";
    }

    return(ArgumentsHandler::_options.at(option));
}
