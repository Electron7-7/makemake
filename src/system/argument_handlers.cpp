#include "argument_handlers.hpp"
#include "arguments/arguments.hpp"
#include "common/labels.hpp"
#include <cstdio>

ErrCode helper_CheckOptionHasValue(const Option& option, bool silence_printout = true, const std::string& treat_as_warning_and_show_this_as_the_default_value = "")
{
    if(option.HasValue())
        return Err::NO_ERROR;

    if(!silence_printout)
    {
        const char* print_label = ERROR;
        std::string warning_message = "\n";

        if(!treat_as_warning_and_show_this_as_the_default_value.empty())
        {
            print_label = WARN;
            warning_message = " Defaulting to " + treat_as_warning_and_show_this_as_the_default_value + "\n";
        }

        printf("%s OptionsHandler - Option [%s, %s] wasn't given a value%s%s", print_label, option.ShortName(), option.LongName(), warning_message.c_str(), COLOR_RESET);
    }

    return Err::Args::ARGUMENT_OPTION_MISSING;
}

ErrCode OptionsHandler(std::vector<Option>* options)
{
    for(const Option& option : *options)
    {
        if(option == Options::SourceDirectory)
        {
            if(helper_CheckOptionHasValue(option, false, global_SourceCodeDirectory) == Err::NO_ERROR)
                global_SourceCodeDirectory = option.GetValue();

            continue;
        }

        if(option == Options::ProgramName)
        {
            if(helper_CheckOptionHasValue(option, false, "the name of the current working directory"))
                global_ProgramName = option.GetValue();

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

        if(flag == Flags::DryRun)
        {
            global_DryRun = true;
            continue;
        }

        if(flag == Flags::debug_NoPrintout)
        {
            _global_DebugNoPrintOut = true;
            continue;
        }
    }

    return Err::NO_ERROR;
}
