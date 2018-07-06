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

#ifndef SRC_UTIL_DURATION_HPP_
#define SRC_UTIL_DURATION_HPP_

#include <chrono>
#include <cstdint>

namespace sctf
{
namespace util
{
/**
 * @brief Measure time in milliseconds.
 * @note The start timepoint is fixed upon construction.
 */
struct Duration final
{
    /**
     * @brief Constructor
     */
    Duration() : _start(std::chrono::steady_clock::now())
    {}

    /**
     * @brief Destructor
     */
    ~Duration() noexcept
    {}

    /**
     * @brief Get duration since start time in milliseconds.
     * @return the milliseconds
     */
    double get()
    {
        return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now()
                                                         - _start)
            .count();
    }

private:
    /// @brief start timepoint
    const std::chrono::steady_clock::time_point _start;
};

}  // namespace util

}  // namespace sctf

#endif  // SRC_UTIL_DURATION_HPP_
