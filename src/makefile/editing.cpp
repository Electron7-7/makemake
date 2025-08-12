#include "editing.hpp"
#include "common/error_codes.hpp"
#include "generating.hpp"
#include "data_types.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>

bool try_UpdateSourceDirectories()
{
    SafeReturn replacement_string = try_GetSourceDirectories();

    // FIXME: Repeated code
    if(replacement_string.ErrorCode() == Err::Generating::SOURCE_DIR_INVALID)
        return false;

    std::fstream makefile;
    std::stringstream makefile_string_stream;
    std::string makefile_string;

    makefile.open("Makefile");
    makefile_string_stream << makefile.rdbuf();
    makefile_string = makefile_string_stream.str();
    makefile.close();

    size_t erase_start = makefile_string.find("SRC_DIRS :=");

    std::string line_buffer = "";

    size_t iterator = erase_start;

    for(; iterator < makefile_string.size(); iterator++)
    {
        char current_character = makefile_string.at(iterator);

        if(current_character != '\n')
        {
            line_buffer += current_character;
            continue;
        }

        if(line_buffer.contains("$(SRC)/") || line_buffer.starts_with("SRC_DIRS :="))
        {
            line_buffer.clear();
            continue;
        }

        iterator -= line_buffer.size();
        break;
    }

    size_t erase_end = iterator - 1;

    std::string trimmed_replacement_string = replacement_string.Data().GetLine();

    /*
        * Quick Description for why I use `pop_back()` 1 or 2 times:
        * The replacement string ends with two empty lines. The first one always gets
        * removed, but the second one only gets removed if the character right after
        * the "to be replaced" selection is also a new line. The way I've formatted
        * these directory entries adds a backslash to the end of the last entry, and
        * `make` will throw an error if there isn't an empty line immediately after.
    */
    trimmed_replacement_string.pop_back();
    if(makefile_string.at(erase_end + 1) == '\n') trimmed_replacement_string.pop_back();

    makefile_string.erase(erase_start, (erase_end - erase_start));
    makefile_string.insert(erase_start, trimmed_replacement_string);

    std::filesystem::remove(std::filesystem::path("Makefile"));
    makefile.open("Makefile", std::ios_base::out);
    makefile << makefile_string;
    makefile.close();
    return true;
}
