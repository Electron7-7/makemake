#ifndef MAKEFILE_GENERATOR_H
#define MAKEFILE_GENERATOR_H

#include "common/error_codes.hpp"
#include <string>

SafeReturn<std::string> GenerateDefaultMakefile();

#endif // MAKEFILE_GENERATOR_H
