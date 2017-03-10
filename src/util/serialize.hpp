#ifndef UTIL_SERIALIZE_HPP_
#define UTIL_SERIALIZE_HPP_

#include <string>

namespace serialize
{

inline std::string serialize(const char*& arg)
{
    return std::string(arg);
}

inline std::string serialize(const int& arg)
{
    return std::to_string(arg);
}

inline std::string serialize(const double& arg)
{
    return std::to_string(arg);
}

inline std::string serialize(const std::string& arg)
{
    return arg;
}

}

#endif /* UTIL_SERIALIZE_HPP_ */
