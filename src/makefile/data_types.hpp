#ifndef MAKEFILE_DATA_TYPES_H
#define MAKEFILE_DATA_TYPES_H

#include <vector>
#include <string>

struct make_variable_t
{
public:
    make_variable_t(const std::string& Name, const std::string& Value, const std::string& Sign, bool IsExported = false);

    bool IsExported() const;
    std::string GetLine() const;

private:
    std::string name = "";
    std::string value = "";
    std::string sign = "";
    bool is_export = false;
};

struct make_target_t
{
public:
    make_target_t(const std::string& Name, const std::vector<std::string>& Value);

    std::string GetLines(bool auto_indent = true) const;

private:
    std::string name = "";
    std::vector<std::string> value = {};
};

#endif // MAKEFILE_DATA_TYPES_H
