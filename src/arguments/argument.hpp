#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "common/compare_strings.hpp"
#include "common/debugging.hpp"

struct _Arg
{
public:
    _Arg(const char* LongName, const char* ShortName): _Arg(uids++, LongName, ShortName) {}
    _Arg(const char* Name): _Arg(-1, Name, Name) {}
    // _Arg(): _Arg(-1, "", "") {}
    _Arg(const _Arg& CopyFrom): _Arg(CopyFrom._uid, CopyFrom._long_name, CopyFrom._short_name) {}

    const char* LongName() const { return _long_name; }
    const char* ShortName() const { return _short_name; }
    int UID() const { return _uid; }

    const bool operator==(const _Arg& other) const
    {
        if(_uid > -1 && other._uid > -1)
            return (_uid == other._uid);

        return (CompareStrings(_long_name, other._long_name) || CompareStrings(_short_name, other._short_name));
    }

    const bool operator!=(const _Arg& other) const { return !(*this == other);   }
    const bool operator< (const _Arg& other) const { return (_uid < other._uid); }
    const bool operator> (const _Arg& other) const { return (_uid > other._uid); }

protected:
    _Arg(int UID, const char* LongName, const char* ShortName): _uid(UID), _long_name(LongName), _short_name(ShortName) {}

    inline static unsigned int uids = 0;
    int _uid;
    const char* _long_name  = "";
    const char* _short_name = "";
};

#endif // ARGUMENT_H
