#include "arguments/arguments.hpp"
#include "arguments/arguments_parser.hpp"
#include "argument_handlers.hpp"
#include "prototype_makefile_generator.hpp"
#include <fstream>

int main(int argc, char** argv)
{
    // Add valid flags
    global_ArgumentsParser->AddFlag(Flags::Help);
    global_ArgumentsParser->AddFlag(Flags::Version);

    // Add valid options
    global_ArgumentsParser->AddOption(Options::DebugPrint);

    // Parse all arguments
    global_ArgumentsParser->ParseArguments(argc, argv);

    // Handle flags
    if(unsigned short return_value = FlagsHandler(global_ArgumentsParser->GetFlags()) != Err::NO_ERROR)
        return return_value;

    // Handle options
    if(unsigned short return_value = OptionsHandler(global_ArgumentsParser->GetOptions()) != Err::NO_ERROR)
        return return_value;

    std::string makefile_data = prototype_GenerateDefaultMakefile(source_directory);
    std::ofstream makefile("Makefile");
    makefile << makefile_data;
    makefile.close();

    return 0;
}
