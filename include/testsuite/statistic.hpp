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

#ifndef SCTF_TESTSUITE_STATISTICS_HPP
#define SCTF_TESTSUITE_STATISTICS_HPP

#include <cstddef>

namespace sctf
{
namespace intern
{
/**
 * Used for storing results per testsuite.
 */
class statistic
{
public:
    /**
     * Get the total number of tests.
     */
    inline auto
    tests() const -> std::size_t {
        return m_num_tests;
    }

    /**
     * Get the number of successful tests.
     */
    inline auto
    successes() const -> std::size_t {
        return m_num_tests - m_num_errs - m_num_fails;
    }

    /**
     * Get the number of failed tests.
     */
    inline auto
    failures() const -> std::size_t {
        return m_num_fails;
    }

    /**
     * Get the number of erroneous tests.
     */
    inline auto
    errors() const -> std::size_t {
        return m_num_errs;
    }

private:
    friend class testsuite;
    friend class testsuite_parallel;

    std::size_t m_num_tests = 0;  ///< Total number of tests.
    std::size_t m_num_fails = 0;  ///< Number of failed tests.
    std::size_t m_num_errs  = 0;  ///< Number of erroneous tests.
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_TESTSUITE_STATISTICS_HPP
