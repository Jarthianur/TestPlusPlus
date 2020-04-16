/*
    Copyright (C) 2017 Jarthianur

    This file is part of simple-cpp-test-framework.

    simple-cpp-test-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple-cpp-test-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple-cpp-test-framework.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SCTF_ASSERTION_FAILURE_HPP
#define SCTF_ASSERTION_FAILURE_HPP

#include <exception>
#include <string>

#include "loc.hpp"

namespace sctf
{
namespace intern
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

    virtual ~assertion_failure() noexcept override = default;

    /**
     * Get the failure message.
     */
    inline char const* what() const noexcept override {
        return m_msg.c_str();
    }

protected:
    std::string const m_msg;  ///< Message describing the failure cause.
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_ASSERTION_FAILURE_HPP
