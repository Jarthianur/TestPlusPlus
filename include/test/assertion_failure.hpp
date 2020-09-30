/*
    Copyright (C) 2017 Jarthianur

    This file is part of TestPlusPlus (Test++).

    TestPlusPlus (Test++) is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TestPlusPlus (Test++) is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TestPlusPlus (Test++).  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TPP_TEST_ASSERTION_FAILURE_HPP
#define TPP_TEST_ASSERTION_FAILURE_HPP

#include <exception>
#include <string>

#include "test/loc.hpp"

namespace tpp
{
namespace intern
{
namespace test
{
/**
 * Used to be thrown when an assertion fails.
 */
class assertion_failure : public std::exception
{
public:
    /**
     * @param msg_ is the failure message
     * @param loc_ is the line of code where the assertion took place
     */
    assertion_failure(std::string const& msg_, loc const& loc_)
        : m_msg(msg_ + " at " + loc_.file + ":" + std::to_string(loc_.line)) {}

    /**
     * Get the failure message.
     */
    inline auto
    what() const noexcept -> char const* override {
        return m_msg.c_str();
    }

private:
    std::string const m_msg;  ///< Message describing the failure cause.
};
}  // namespace test
}  // namespace intern
}  // namespace tpp

#endif  // TPP_TEST_ASSERTION_FAILURE_HPP
