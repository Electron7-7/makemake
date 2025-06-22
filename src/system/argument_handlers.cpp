#include "argument_handlers.hpp"
#include "arguments/arguments.hpp"
#include "common/printouts.hpp"

ErrorCode OptionsHandler(std::vector<Option>* options)
{
    for(const Option& option : *options)
    {
        if(option == Options::GenerateMakefile)
        {
            if(!option.HasValue())
            {
                PRINTERR("OptionsHandler - Option [" + option.ShortName() + ", " + option.LongName() + "] is missing its mandatory required value");
                return Err::Args::MANDATORY_INPUT_MISSING;
            }

            continue;
        }

        if(option == Options::DebugPrint)
        {
            PRINT("DebugPrint: " + option.GetValue());
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
            PRINT(_Help_Printout);
            continue;
        }

        if(flag == Flags::Version)
        {
            PRINT(_Version_Printout);
            continue;
        }
    }

    return Err::NO_ERROR;
}
