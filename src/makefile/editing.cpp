#include "editing.hpp"
#include "common/error_codes.hpp"
#include "generating.hpp"
#include "data_types.hpp"

#include <fstream>
#include <sstream>

bool try_UpdateSourceDirectories()
{
    SafeReturn replacement_string = try_GetSourceDirectories();

    // FIXME: Repeated code
    if(replacement_string.ErrorCode() == Err::Generating::SOURCE_DIR_INVALID)
        return 1;

    std::fstream makefile;
    std::stringstream makefile_string_stream;
    std::string makefile_string;

    makefile.open("Makefile");
    makefile_string_stream << makefile.rdbuf();
    makefile_string = makefile_string_stream.str();

    size_t erase_start = makefile_string.find("SRC_DIRS :=");
    size_t erase_end = makefile_string.rfind("\\\n", makefile_string.find("CC_SRCS")) + 3;

    makefile_string.erase(erase_start, (erase_end - erase_start));
    makefile_string.insert(erase_start, replacement_string.Data().GetLine());

    makefile.seekp(0);
    makefile << makefile_string;
    makefile.close();
    return 0;
}
