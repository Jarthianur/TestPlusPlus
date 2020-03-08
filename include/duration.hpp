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

#ifndef SCTF_COMMON_DURATION_HPP
#define SCTF_COMMON_DURATION_HPP

#include <chrono>

namespace sctf
{
namespace private_
{
/**
 * Used to measure durations in milliseconds.
 * The starting timepoint is fixed on construction.
 */
class duration final
{
public:
    duration() : m_start(std::chrono::steady_clock::now()) {}

    ~duration() noexcept = default;

    /**
     * Get the duration since start in milliseconds.
     */
    double get() {
        return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - m_start)
            .count();
    }

private:
    std::chrono::steady_clock::time_point const m_start;
};
}  // namespace private_
}  // namespace sctf

#endif  // SCTF_COMMON_DURATION_HPP
