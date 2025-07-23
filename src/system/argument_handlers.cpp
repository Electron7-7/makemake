#include "argument_handlers.hpp"
#include "arguments/arguments.hpp"
#include "common/labels.hpp"

ErrCode OptionsHandler(std::vector<Option>* options)
{
    for(const Option& option : *options)
    {
        if(option == Options::SourceDirectory)
        {
            if(!option.HasValue())
            {
                printf("%s OptionsHandler - Option [%s, %s] is missing its mandatory required value\n%s", ERROR, option.ShortName(), option.LongName(), COLOR_RESET);
            }

            source_directory = option.GetValue();
            continue;
        }

        if(option == Options::ProgramName)
        {
            if(!option.HasValue())
            {
                printf("%s OptionsHandler - Option [%s, %s] is missing its mandatory required value\n%s", ERROR, option.ShortName(), option.LongName(), COLOR_RESET);
            }

            program_name = option.GetValue();
            continue;
        }
    }

    return Err::NO_ERROR;
}

ErrCode FlagsHandler(std::vector<Flag>* flags)
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
