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

#ifndef SCTF_TESTSUITE_STATISTICS_HPP
#define SCTF_TESTSUITE_STATISTICS_HPP

#include <cstddef>

namespace sctf
{
namespace intern
{
/**
 * Used for storing test results.
 */
class statistic
{
public:
    /**
     * Get the absolute number of tests.
     */
    inline std::size_t tests() const {
        return m_num_of_tests;
    }

    /**
     * Get the number of successful tests.
     */
    inline std::size_t successes() const {
        return m_num_of_tests - m_num_of_errs - m_num_of_fails;
    }

    /**
     * Get the number of failed tests.
     */
    inline std::size_t failures() const {
        return m_num_of_fails;
    }

    /**
     * Get the number of erroneous tests.
     */
    inline std::size_t errors() const {
        return m_num_of_errs;
    }

protected:
    friend class testsuite;
    friend class testsuite_parallel;

    std::size_t m_num_of_tests = 0;
    std::size_t m_num_of_fails = 0;
    std::size_t m_num_of_errs  = 0;
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_TESTSUITE_STATISTICS_HPP
