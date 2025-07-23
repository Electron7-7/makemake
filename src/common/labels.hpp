#ifndef LABELS_H
#define LABELS_H

#include "colors.hpp"

constexpr const char* ERROR = COLOR_BOLD(RED) "[ERROR]";
constexpr const char* WARN  = COLOR_BOLD(YELLOW) "[WARNING]";
constexpr const char* DEBUG = COLOR_BOLD(DEFAULT) "[DEBUG]";

#endif // LABELS_H
