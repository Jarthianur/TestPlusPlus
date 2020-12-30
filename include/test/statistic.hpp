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

#ifndef TPP_TEST_STATISTICS_HPP
#define TPP_TEST_STATISTICS_HPP

#include <cstddef>

namespace tpp
{
namespace intern
{
namespace test
{
class statistic
{
public:
    inline auto
    tests() const -> std::size_t {
        return m_num_tests;
    }

    inline auto
    successes() const -> std::size_t {
        return m_num_tests - m_num_errs - m_num_fails;
    }

    inline auto
    failures() const -> std::size_t {
        return m_num_fails;
    }

    inline auto
    errors() const -> std::size_t {
        return m_num_errs;
    }

    inline auto
    elapsed_time() const -> double {
        return m_elapsed_t;
    }

private:
    friend class testsuite;
    friend class testsuite_parallel;

    std::size_t m_num_tests{0};
    std::size_t m_num_fails{0};
    std::size_t m_num_errs{0};
    double      m_elapsed_t{.0};
};
}  // namespace test
}  // namespace intern
}  // namespace tpp

#endif  // TPP_TEST_STATISTICS_HPP
