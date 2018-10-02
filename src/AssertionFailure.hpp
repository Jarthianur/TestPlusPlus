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

#ifndef SCTF_SRC_ASSERTIONFAILURE_HPP_
#define SCTF_SRC_ASSERTIONFAILURE_HPP_

#include <exception>
#include <string>

namespace sctf
{
/**
 * @brief Thrown when an assertion fails.
 */
class AssertionFailure : public std::exception
{
public:
    /**
     * @brief Constructor
     * @param msg The base error message
     * @param file The source file where the assertion took place
     * @param line The source line of the assertion
     */
    AssertionFailure(const std::string& msg, const char* file, int line)
        : std::exception(), m_msg(msg + " at " + file + ":" + std::to_string(line))
    {}

    /**
     * @brief Destructor
     */
    ~AssertionFailure() noexcept = default;

    /**
     * @brief Get the error message.
     * @return the message
     */
    inline const char* what() const noexcept override
    {
        return m_msg.c_str();
    }

private:
    /// @brief The error message
    const std::string m_msg;
};

}  // namespace sctf

#endif  // SCTF_SRC_ASSERTIONFAILURE_HPP_
