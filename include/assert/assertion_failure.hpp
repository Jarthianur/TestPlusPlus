/*
    Copyright (C) 2017  Jarthianur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TPP_ASSERT_ASSERTION_FAILURE_HPP
#define TPP_ASSERT_ASSERTION_FAILURE_HPP

#include <exception>
#include <string>

#include "assert/loc.hpp"

namespace tpp
{
namespace intern
{
namespace assert
{
class assertion_failure : public std::exception
{
public:
    assertion_failure(std::string const& msg_, loc const& loc_)
        : m_msg(msg_ + " at " + loc_.file + ":" + std::to_string(loc_.line)) {}

    inline auto
    what() const noexcept -> char const* override {
        return m_msg.c_str();
    }

private:
    std::string const m_msg;
};
}  // namespace assert
}  // namespace intern
}  // namespace tpp

#endif  // TPP_ASSERT_ASSERTION_FAILURE_HPP
