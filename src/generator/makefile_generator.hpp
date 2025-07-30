#ifndef MAKEFILE_GENERATOR_H
#define MAKEFILE_GENERATOR_H

#include "common/error_codes.hpp"

struct make_variable_t; // Forward Declaration

SafeReturn<const char*> try_GenerateDefaultMakefile();
SafeReturn<make_variable_t> try_GetSourceDirectories();

const char* GetSourceDirectories();

#endif // MAKEFILE_GENERATOR_H
