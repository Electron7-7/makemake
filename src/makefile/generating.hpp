#ifndef GENERATING_H
#define GENERATING_H

#include "common/error_codes.hpp"

struct make_variable_t; // Forward Declaration

SafeReturn<const char*> try_GenerateDefaultMakefile();
SafeReturn<make_variable_t> try_GetSourceDirectories(); // TODO: Move this out of here

const char* GetSourceDirectories();

#endif // GENERATING_H
