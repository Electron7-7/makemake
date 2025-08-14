#ifndef MAKEFILE_DATA_TYPES_H
#define MAKEFILE_DATA_TYPES_H

#include <vector> // TODO: Replace with 'std::list'
#include <string>

struct make_variable_t
{
public:
    make_variable_t(const std::string& Name, const std::string& Value, const std::string& Sign, bool IsExported = false);

    std::string GetLine() const;
    std::string GetValue() const;
    std::string GetName() const;
    std::string GetSign() const;
    bool IsExported() const;

    std::string ClearValue();
    void SetName(const std::string& Name);
    void SetValue(const std::string& Value);
    void AppendValue(const std::string& Value);
    void SetSign(const std::string& Sign);
    void SetIsExported(const bool IsExported);

    static constexpr const char* EMPTY = "# Nothing yet";

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
    const std::vector<std::string>& GetLinesVector() const;

    void SetName(const std::string& Name);
    void AddLine(const std::string& Line);
    size_t GetLinesSize() const;
    bool try_SetLine(const unsigned int Index, const std::string& Line);
    bool try_RemoveLine(const unsigned int Index);

private:
    std::string name = "";
    std::vector<std::string> value = {};
};

#endif // MAKEFILE_DATA_TYPES_H
