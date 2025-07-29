#ifndef MAKEFILE_GENERATOR_H
#define MAKEFILE_GENERATOR_H

#include "common/error_codes.hpp"

SafeReturn<const char*> GenerateDefaultMakefile();

#endif // MAKEFILE_GENERATOR_H
