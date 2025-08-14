#include "interfacing.hpp"
#include "template.hpp"
#include "system/arguments.hpp"
int try_ReplaceVariable(make_variable_t* variable, std::string multiline_check)
{
    if(!CheckMakefileExists()) { return 1; }

    std::fstream makefile;
    std::stringstream makefile_string_stream;
    std::string makefile_string;

    makefile.open(constant_MakefileFileName);
    makefile_string_stream << makefile.rdbuf();
    makefile_string = makefile_string_stream.str();
    makefile.close();

    std::string line_beginning( variable->GetName() + (variable->GetSign().substr(0, 3)) );

    size_t erase_start = makefile_string.find(line_beginning);
    size_t erase_end = erase_start;

    if(erase_start == std::string::npos || makefile_string.at(erase_start - 1) != '\n')
    { return 1; }

    size_t iterator = (erase_start - 1);
    size_t size_of_makefile = makefile_string.size();
    std::string line_buffer = "";
    char current_character = ' ';

    if(multiline_check.empty()) // Single-line variable process
    {
        PRINT_DEBUG("Processing a single-line variable")
        while(current_character != '\n' && ++iterator < size_of_makefile)
        {
            current_character = makefile_string.at(iterator);
            line_buffer += current_character;
        }

        erase_end = iterator;
    }
    else // Multi-line variable process
    {
        PRINT_DEBUG("Processing a multi-line variable")
        while(++iterator < size_of_makefile)
        {
            current_character = makefile_string.at(iterator);

            if(current_character != '\n')
            {
                line_buffer += current_character;
                continue;
            }

            if(line_buffer.starts_with(line_beginning) || line_buffer.contains(multiline_check))
            {
                line_buffer.clear();
                continue;
            }

            iterator -= line_buffer.size();
            break;
        }

        erase_end = (iterator - 1);
    }

    if(erase_end <= erase_start)
    {
        PRINT_ERROR("try_ReplaceVariable - 'erase_end <= erase_start' returned true! Potential issue: 'try_ReplaceVariable' was told to process a multiline variable, but only read one line.")
        return 1;
    }

    std::string trimmed_variable = variable->GetLine();
    std::string check_for_newlines((trimmed_variable.end() - 2).base());

    /*
        * Quick Description for why I use `pop_back()` 1 or 2 times:
        * The replacement string ends with two empty lines. The first one always gets
        * removed, but the second one only gets removed if the character right after
        * the "to be replaced" selection is also a new line. The way I've formatted
        * these directory entries adds a backslash to the end of the last entry, and
        * `make` will throw an error if there isn't an empty line immediately after.
    */

    if(check_for_newlines.at(1) == '\n')
    { trimmed_variable.pop_back(); }

    if(check_for_newlines.at(0) == '\n' && makefile_string.at(erase_end + 1) == '\n')
    { trimmed_variable.pop_back(); }

    PRINT_DEBUG("Erasing: '{}{}{}{}'", Color::Back::RedBold, Color::Front::WhiteBold, makefile_string.substr(erase_start, (erase_end - erase_start)), Color::Reset);

    if(Flags::DryRun.IsActive())
    {
        std::print("{}\n", trimmed_variable);
        return 0;
    }

    PRINT_DEBUG("Inserting: '{}{}{}{}'", Color::Back::GreenBold, Color::Front::WhiteBold, trimmed_variable, Color::Reset);

    makefile_string.erase(erase_start, (erase_end - erase_start));
    makefile_string.insert(erase_start, trimmed_variable);

    std::filesystem::remove(std::filesystem::path(constant_MakefileFileName));

    makefile.open(constant_MakefileFileName, std::ios_base::out);
    makefile << makefile_string;
    makefile.close();

    return 0;
}
