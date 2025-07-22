#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "common/compare_strings.hpp"
#include <string>

struct _Arg
{
public:
    constexpr _Arg(const char* LongName, const char* ShortName):_long_name(LongName), _short_name(ShortName) {}
    constexpr _Arg(const _Arg& CopyFrom): _Arg(CopyFrom._long_name, CopyFrom._short_name) {}
    constexpr _Arg(const char* Name): _Arg(Name, Name) {}

    constexpr const char* LongName()  const { return _long_name;  }
    constexpr const char* ShortName() const { return _short_name; }

    constexpr bool operator==(const _Arg& other) const { return (CompareStrings(_long_name, other._long_name) || CompareStrings(_short_name, other._short_name)); }
    constexpr bool operator!=(const _Arg& other) const { return !(*this == other);   }

protected:
    const char* _long_name = "";
    const char* _short_name = "";
};

struct Flag : public _Arg
{
public:
    using _Arg::_Arg;

    void Activate() { _active = true; }
    bool IsActive() const { return _active; }

private:
    bool _active = false;
};

struct Option : public _Arg
{
public:
    using _Arg::_Arg;

    void SetValue(const char* Value) { _value = Value; }
    const char* GetValue() const { return _value; }
    bool HasValue() const { return (!CompareStrings(_value, "")); } // FIXME: Kind of redundant, since I can just check that 'GetValue' isn't an empty string...

private:
    const char* _value = "";
};

#endif // ARGUMENT_H
