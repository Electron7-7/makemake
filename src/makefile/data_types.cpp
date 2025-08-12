#include "data_types.hpp"

make_variable_t::make_variable_t(const std::string& Name, const std::string& Value, const std::string& Sign, bool IsExported)
: name(Name), value(Value), sign(Sign), is_export(IsExported)
{}

std::string make_variable_t::GetLine() const
{
    std::string return_value = name + sign + value + "\n";
    if(is_export)
        return_value = "export " + return_value;
    return return_value;
}

std::string make_variable_t::GetValue() const
{ return value; }

std::string make_variable_t::GetName() const
{ return name; }

std::string make_variable_t::GetSign() const
{ return sign; }

bool make_variable_t::IsExported() const
{ return is_export; }

std::string make_variable_t::ClearValue()
{
    if(!value.compare(make_variable_t::EMPTY))
    { value = ""; }
    return value;
}

void make_variable_t::SetName(const std::string& new_name)      { name = new_name; }
void make_variable_t::SetValue(const std::string& new_value)    { value = new_value; }
void make_variable_t::AppendValue(const std::string& new_value) { value += " " + new_value; }
void make_variable_t::SetSign(const std::string& new_sign)      { sign = new_sign; }
void make_variable_t::SetIsExported(const bool is_exported)     { is_export = is_exported; }

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

const std::vector<std::string>& make_target_t::GetLinesVector() const
{ return value; }

void make_target_t::SetName(const std::string& new_name) { name = new_name; }

bool make_target_t::try_SetLine(const unsigned int index, const std::string& new_line)
{
    if(value.size() <= index)
        return false;

    value.at(index) = new_line;
    return true;
}

size_t make_target_t::GetLinesSize() const
{ return value.size(); }

void make_target_t::AddLine(const std::string& new_line)
{
    value.insert(value.end(), new_line);
}

bool make_target_t::try_RemoveLine(const unsigned int index)
{
    if(value.size() <= index)
        return false;

    value.erase(value.cbegin() + index);
    return true;
}
