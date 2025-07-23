#include "data_types.hpp"

make_variable_t::make_variable_t(const std::string& Name, const std::string& Value, const std::string& Sign, bool IsExported)
: name(Name), value(Value), sign(Sign), is_export(IsExported)
{}

bool make_variable_t::IsExported() const
{ return is_export; }

std::string make_variable_t::GetLine() const
{
    std::string return_value = name + sign + value + "\n";
    if(is_export)
        return_value = "export " + return_value;
    return return_value;
}

make_target_t::make_target_t(const std::string& Name, const std::vector<std::string>& Value)
: name(Name), value(Value)
{}

std::string make_target_t::GetLines(bool auto_indent) const
{
    std::string return_value = name + "\n";
    for(std::string line : value)
    {
        if(auto_indent)
            return_value += "\t";
        return_value += line + "\n";
    }
    return return_value + "\n";
}
