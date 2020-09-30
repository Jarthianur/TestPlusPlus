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

#ifndef TPP_TEST_DURATION_HPP
#define TPP_TEST_DURATION_HPP

#include <chrono>

namespace tpp
{
namespace intern
{
namespace test
{
/**
 * Used to measure durations in milliseconds.
 * The starting timepoint is fixed upon construction.
 */
class duration final
{
public:
    duration() : m_start(std::chrono::steady_clock::now()) {}

    /**
     * Get the duration since starting point in milliseconds.
     */
    auto
    get() -> double {
        return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - m_start).count();
    }

private:
    std::chrono::steady_clock::time_point const m_start;  ///< Starting timepoint.
};
}  // namespace test
}  // namespace intern
}  // namespace tpp

#endif  // TPP_TEST_DURATION_HPP
