#ifndef ARGUMENT_HANDLERS_H
#define ARGUMENT_HANDLERS_H

#include "common/error_codes.hpp"
#include "arguments/argument.hpp"
#include <vector>

ErrorCode OptionsHandler(std::vector<Option>* Options);
ErrorCode FlagsHandler(std::vector<Flag>* Flags);

#endif // ARGUMENT_HANDLERS_H
