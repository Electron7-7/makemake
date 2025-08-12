#ifndef COLORS_H
#define COLORS_H

struct Color
{
    constexpr static const char* Reset = "\x1b[0m";

    struct Front
    {
        constexpr static const char* Black   = "\x1b[30m";
        constexpr static const char* Red     = "\x1b[31m";
        constexpr static const char* Green   = "\x1b[32m";
        constexpr static const char* Yellow  = "\x1b[33m";
        constexpr static const char* Blue    = "\x1b[34m";
        constexpr static const char* Magenta = "\x1b[35m";
        constexpr static const char* Cyan    = "\x1b[36m";
        constexpr static const char* White   = "\x1b[37m";
        constexpr static const char* Default = "\x1b[39m";

        constexpr static const char* BlackBold   = "\x1b[1;30m";
        constexpr static const char* RedBold     = "\x1b[1;31m";
        constexpr static const char* GreenBold   = "\x1b[1;32m";
        constexpr static const char* YellowBold  = "\x1b[1;33m";
        constexpr static const char* BlueBold    = "\x1b[1;34m";
        constexpr static const char* MagentaBold = "\x1b[1;35m";
        constexpr static const char* CyanBold    = "\x1b[1;36m";
        constexpr static const char* WhiteBold   = "\x1b[1;37m";
        constexpr static const char* DefaultBold = "\x1b[1;39m";
    };

    struct Back
    {
        constexpr static const char* Black   = "\x1b[40m";
        constexpr static const char* Red     = "\x1b[41m";
        constexpr static const char* Green   = "\x1b[42m";
        constexpr static const char* Yellow  = "\x1b[43m";
        constexpr static const char* Blue    = "\x1b[44m";
        constexpr static const char* Magenta = "\x1b[45m";
        constexpr static const char* Cyan    = "\x1b[46m";
        constexpr static const char* White   = "\x1b[47m";
        constexpr static const char* Default = "\x1b[49m";

        constexpr static const char* BlackBold   = "\x1b[2;40m";
        constexpr static const char* RedBold     = "\x1b[2;41m";
        constexpr static const char* GreenBold   = "\x1b[2;42m";
        constexpr static const char* YellowBold  = "\x1b[2;43m";
        constexpr static const char* BlueBold    = "\x1b[2;44m";
        constexpr static const char* MagentaBold = "\x1b[2;45m";
        constexpr static const char* CyanBold    = "\x1b[2;46m";
        constexpr static const char* WhiteBold   = "\x1b[2;47m";
        constexpr static const char* DefaultBold = "\x1b[2;49m";
    };
};

#endif // COLORS_H