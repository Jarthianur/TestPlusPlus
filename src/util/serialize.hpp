/*
 Copyright_License {

 Copyright (C) 2017 Julian P. Becht
 Author: Julian P. Becht

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License version 3
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 }
 */

#ifndef UTIL_SERIALIZE_HPP_
#define UTIL_SERIALIZE_HPP_

#include <string>

namespace testsuite
{
namespace util
{
/**
 * Serialize number types.
 */
template<typename T>
inline std::string serialize(const T& arg)
{
    return std::to_string(arg);
}

/**
 * Serialize strings.
 */
template<>
inline std::string serialize(const std::string& arg)
{
    return arg;
}

/**
 * Serialize const char ptr's
 */
template<>
inline std::string serialize(const char* const& arg)
{
    return std::string(arg);
}

}  // util
}  // testsuite

#endif /* UTIL_SERIALIZE_HPP_ */
