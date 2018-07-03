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

#ifndef COMPARATOR_COMPARATORS_HPP_
#define COMPARATOR_COMPARATORS_HPP_

#include <memory>
#include <string>

/// @namespace testsuite
namespace testsuite
{
/// @namespace comparator
namespace comparator
{
/**
 * Workaround for the lack of optionals
 *
 * @brief The Comparison struct
 */
struct Comparison
{
    constexpr Comparison() : success(true)
    {}

    explicit Comparison(const std::string& comp, const std::string& val,
                        const std::string& exp)
        : success(false)
    {
        std::string msg;
        msg.reserve(15 + comp.length() + val.length() + exp.length());
        msg = "Expected '";
        msg.append(val).append("' ").append(comp).append(" '").append(exp).append("'");
        error() = msg;
    }

    explicit operator bool()
    {
        return success;
    }

    const std::string& operator*() const
    {
        return error();
    }

private:
    const bool success;

    std::string& error() const
    {
        static thread_local std::string err_msg;
        return err_msg;
    }
};

/**
 * Template comparator type as shared ptr.
 * T: type to compare (e.g. int)
 */
template<typename T>
using Comparator = Comparison (*)(const T&, const T&);

constexpr Comparison success = Comparison();

}  // namespace comparator
}  // namespace testsuite

#define PROVIDE_COMPARATOR(COMP, NAME)                        \
    template<typename T>                                      \
    inline static testsuite::comparator::Comparator<T> NAME() \
    {                                                         \
        return &testsuite::comparator::COMP<T>;               \
    }

#endif /* COMPARATOR_COMPARATORS_HPP_ */
