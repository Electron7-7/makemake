#ifndef PROTOTYPE_MAKEFILE_GENERATOR_H
#define PROTOTYPE_MAKEFILE_GENERATOR_H

#include "common/error_codes.hpp"
#include <string>

SafeReturn<std::string> prototype_GenerateDefaultMakefile(const std::string& SourceDirectory);

#endif
