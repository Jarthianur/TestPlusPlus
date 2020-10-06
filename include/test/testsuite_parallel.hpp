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

#ifndef TPP_TEST_TESTSUITE_PARALLEL_HPP
#define TPP_TEST_TESTSUITE_PARALLEL_HPP

#include <cstdint>
#include <limits>
#include <stdexcept>

#include "test/testsuite.hpp"

namespace tpp
{
namespace intern
{
namespace test
{
/**
 * Group testcases together in a testsuite. Testcases share this testsuite as their context.
 * Testcases are run in parallel.
 */
class testsuite_parallel : public testsuite
{
public:
    /**
     * Create a new testsuite.
     *
     * @param name_ is the name, or description of the testsuite.
     */
    static auto
    create(char const* name_) -> testsuite_ptr {
        return std::make_shared<testsuite_parallel>(enable{}, name_);
    }

    /**
     * Run all testcases in this suite.
     * @throw std::overflow_error if there are too many testcases for openmp to handle.
     */
    void
    run() override {
        if (m_state != IS_DONE) {
            duration d;
            if (m_testcases.size() > static_cast<std::size_t>(std::numeric_limits<std::int64_t>::max())) {
                throw std::overflow_error("Too many testcases! Size would overflow loop variant.");
            }
            auto const tc_size{static_cast<std::int64_t>(m_testcases.size())};
            m_stats.m_num_tests = m_testcases.size();
            streambuf_proxies<streambuf_proxy_omp> bufs;
            m_setup_fn();
#pragma omp parallel default(shared)
            {  // BEGIN parallel section
                std::size_t fails{0};
                std::size_t errs{0};
                // OpenMP 2 compatible - MSVC not supporting higher version
#pragma omp for schedule(dynamic)
                for (std::int64_t i = 0; i < tc_size; ++i) {
                    auto& tc{m_testcases[static_cast<std::size_t>(i)]};
                    if (tc.result() == testcase::IS_UNDONE) {
                        m_pretest_fn();
                        tc();
                        switch (tc.result()) {
                            case testcase::HAS_FAILED: ++fails; break;
                            case testcase::HAD_ERROR: ++errs; break;
                            default: break;
                        }
                        m_posttest_fn();
                        tc.cout(bufs.cout.str());
                        tc.cerr(bufs.cerr.str());
                    }
                }
#pragma omp critical
                {  // BEGIN critical section
                    m_stats.m_num_fails += fails;
                    m_stats.m_num_errs += errs;
                }  // END critical section
            }      // END parallel section
            m_teardown_fn();
            m_state = IS_DONE;
            m_stats.m_elapsed_t += d.get();
        }
    }

    /**
     * Constructor for std::make_shared.
     *
     * @param name_ is the name, or description of the testsuite.
     */
    testsuite_parallel(enable e_, char const* name_) : testsuite(e_, name_) {}
};
}  // namespace test
}  // namespace intern
}  // namespace tpp

#endif  // TPP_TEST_TESTSUITE_PARALLEL_HPP
