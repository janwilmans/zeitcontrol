#pragma once

#define FMT_HEADER_ONLY
#include <fmt/format.h>

inline std::string & ltrim(std::string & str, const std::string & chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

inline std::string & rtrim(std::string & str, const std::string & chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

inline std::string strip(std::string str, const std::string & chars = "\t\n\v\f\r ")
{
    ltrim(rtrim(str, chars), chars);
    return str;
}

inline void print_errno(std::string_view cmd)
{
    const auto err = errno;
    fmt::print("{} error: {} ({})\n", cmd, err, strerror(err));
}
