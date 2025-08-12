#ifndef PRINTING_H
#define PRINTING_H

#include "system/arguments.hpp"

#include <print> // IWYU pragma: keep
#include <string>

// IMPORTANT - These labels do not set the text color back to normal! If you use them manually, make sure to do that!
inline std::string __ERROR()
{
    std::string label = "[ERROR]";
    if(!Flags::NoColor.IsActive()) label = "\x1b[1;31m" + label; // Color hardcoded to "bold red"
    return label;
}

inline std::string __WARNING()
{
    std::string label = "[WARNING]";
    if(!Flags::NoColor.IsActive()) label = "\x1b[1;33m" + label; // Color hardcoded to "bold yellow"
    return label;
}

inline std::string __DEBUG()
{
    std::string label = "[DEBUG]";
    if(!Flags::NoColor.IsActive()) label = "\x1b[1;39m" + label; // Color hardcoded to "bold default"
    return label;
}

constexpr const char* __RESET_NL = "\x1b[0m\n"; // Reset all changes & add a newline


/*
    My printout macros are inspired by these ingenious StackOverflow answers:
        https://stackoverflow.com/a/3048361  - Original Concept
        https://stackoverflow.com/a/8814003  - Slight Improvement w/ Explanation
        https://stackoverflow.com/a/28074198 - Major Improvement w/ Explanation

    I give my "greatest respects" to Derek Ledbetter, David Sorkovsky, Syphorlate, Jens Gustedt, and Jason Deng.
    Even if I didn't end up using their work, it was a great inspiration and a really good read.
*/

#define __LABELLED_PRINT(LABEL, FORMAT, ...) std::print("{} " FORMAT "{}", LABEL __VA_OPT__(,) __VA_ARGS__, __RESET_NL);

#define PRINT_ERROR(FORMAT, ARGS...)   __LABELLED_PRINT(__ERROR(), FORMAT, ARGS)
#define PRINT_WARNING(FORMAT, ARGS...) __LABELLED_PRINT(__WARNING(),  FORMAT, ARGS)

#ifdef DEBUGGING // PRINT_DEBUG is disabled by default; add `-D DEBUGGING` to your compile flags to enable it
    #define PRINT_DEBUG(FORMAT, ARGS...) __LABELLED_PRINT(__DEBUG(), FORMAT, ARGS)
#else
    #define PRINT_DEBUG(FORMAT, ARGS...) {} // The curly braces help avoid potential syntax errors
#endif // DEBUGGING

#endif // PRINTING_H
