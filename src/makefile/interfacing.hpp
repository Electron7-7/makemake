#ifndef INTERFACING_H
#define INTERFACING_H

#include <string>

struct make_variable_t; // Forward-declaration

bool try_UpdateSourceDirs();
bool try_SetSourceDirs();
bool try_SetLinkerFlags();
bool try_UpdateLinkerFlags();

// If `MultilineVariableCheck` isn't empty, it's used to determine if a line of text is part of a multiline variable
// (e.g: for 'SRC_DIRS', I pass '$(SRC)/' to 'MultilineVariableCheck')
int try_ReplaceVariable(make_variable_t* Variable, std::string MultilineVariableCheck = "");

const char* GetMakefileBuffer();
void GenerateDefaultMakefile();

extern const char* constant_MakefileFileName;

#endif // INTERFACING_H
