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

#ifndef SCTF_COMMON_TYPES_HPP
#define SCTF_COMMON_TYPES_HPP

#include <functional>
#include <memory>

namespace sctf
{
namespace private_
{
class reporter;

/// Type alias for a parameterless void function, which is used to wrap the actual tests.
using test_function = std::function<void()>;

/**
 * Pack source file and line number together as a location in code.
 */
struct code_location final
{
    char const* file;
    int const   line;
};

/**
 * Provide a singleton pattern implementation.
 * The instance exists in global scope, thus it is not threadsafe by default.
 */
template<typename T>
struct singleton final
{
    /**
     * Get the actual singleton instance.
     * Construction happens only once, on first call with forwarded arguments.
     */
    template<typename... Args>
    static T& instance(Args&&... args_) {
        static T inst(std::forward<Args>(args_)...);
        return inst;
    }
};
}  // namespace private_

class testsuite;
using reporter_ptr  = std::shared_ptr<private_::reporter>;
using testsuite_ptr = std::shared_ptr<testsuite>;
}  // namespace sctf

#endif  // SCTF_COMMON_TYPES_HPP
