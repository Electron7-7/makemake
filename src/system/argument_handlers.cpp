#include "argument_handlers.hpp"
#include "arguments/arguments.hpp"
#include "common/labels.hpp"

ErrorCode OptionsHandler(std::vector<Option>* options)
{
    for(const Option& option : *options)
    {
        if(option == Options::GenerateMakefile)
        {
            if(!option.HasValue())
            {
                printf("%sOptionsHandler - Option [%s, %s] is missing its mandatory required value\n", ERROR, option.ShortName(), option.LongName());
                return Err::Args::MANDATORY_INPUT_MISSING;
            }

            continue;
        }

        if(option == Options::DebugPrint)
        {
            printf("%sDebugPrint: %s\n", DEBUG, option.GetValue());
            continue;
        }
    }

    return Err::NO_ERROR;
}

ErrorCode FlagsHandler(std::vector<Flag>* flags)
{
    for(const Flag& flag : *flags)
    {
        if(flag == Flags::Help)
        {
            printf("%s\n    %s\n", _Help_Printout, _Version_Printout);
            continue;
        }

        if(flag == Flags::Version)
        {
            printf("    %s\n", _Version_Printout);
            continue;
        }
    }

    return Err::NO_ERROR;
}
