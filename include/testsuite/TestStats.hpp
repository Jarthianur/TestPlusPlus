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

#ifndef SCTF_SRC_TESTSUITE_TESTSTATS_HPP_
#define SCTF_SRC_TESTSUITE_TESTSTATS_HPP_

#include <cstddef>

namespace sctf
{
class TestSuite;
class TestSuiteParallel;

namespace _
{
/**
 * @brief Statistics for test results.
 */
class TestStats
{
public:
    /**
     * @brief Constructor
     */
    TestStats() = default;

    /**
     * @brief Destructor
     */
    ~TestStats() noexcept = default;

    /**
     * @brief Get the number of all tests.
     * @return the number of tests
     */
    inline std::size_t tests() const
    {
        return m_num_of_tests;
    }

    /**
     * @brief Get the number of successful tests.
     * @return the number of tests
     */
    inline std::size_t successes() const
    {
        return m_num_of_tests - m_num_of_errs - m_num_of_fails;
    }

    /**
     * @brief Get the number of all failed tests.
     * @return the number of tests
     */
    inline std::size_t failures() const
    {
        return m_num_of_fails;
    }

    /**
     * @brief Get the number of erroneous tests.
     * @return the number of tests
     */
    inline std::size_t errors() const
    {
        return m_num_of_errs;
    }

private:
    friend sctf::TestSuite;
    friend sctf::TestSuiteParallel;

    /// @brief The number of tests.
    std::size_t m_num_of_tests = 0;

    /// @brief The number of failed tests.
    std::size_t m_num_of_fails = 0;

    /// @brief The number of erroneous tests.
    std::size_t m_num_of_errs = 0;
};

}  // namespace _
}  // namespace sctf

#endif  // SCTF_SRC_TESTSUITE_TESTSTATS_HPP_
