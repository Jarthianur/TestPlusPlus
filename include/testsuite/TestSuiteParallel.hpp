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

#ifndef SCTF_SRC_TESTSUITE_TESTSUITEPARALLEL_HPP_
#define SCTF_SRC_TESTSUITE_TESTSUITEPARALLEL_HPP_

#include <iostream>
#include <limits>
#include <stdexcept>

#include "common/streambuf_proxy_omp.hpp"
#include "testsuite/TestSuite.hpp"

namespace sctf
{
/**
 * @brief Testsuite class for managing parallel testcases.
 * @note Non-copyable
 */
class TestSuiteParallel : public TestSuite
{
public:
    TestSuiteParallel(const TestSuiteParallel&) = delete;
    TestSuiteParallel& operator=(const TestSuiteParallel&) = delete;

    /**
     * @brief Create a TestSuiteParallel.
     * @param name The name/description
     * @param context The context
     * @return a shared pointer to the created TestSuiteParallel
     */
    static TestSuite_shared create(const std::string& name, const std::string& context)
    {
        return TestSuite_shared(new TestSuiteParallel(name, context));
    }

    ~TestSuiteParallel() noexcept override = default;

    /**
     * @brief Execute all TestCases in parallel.
     */
    void run() override
    {
        if (m_state != State::DONE)
        {
            if (m_testcases.size() > std::numeric_limits<long>::max())
            {
                throw std::overflow_error("Too many testcases! Size would overflow loop variant.");
            }
            const long tc_size     = static_cast<long>(m_testcases.size());
            m_stats.m_num_of_tests = m_testcases.size();
            _::streambuf_proxy_omp mt_buf_cout(std::cout);
            _::streambuf_proxy_omp mt_buf_cerr(std::cerr);

            SCTF_EXEC_SILENT(m_setup_func)
#pragma omp parallel
            {
                double      tmp   = 0.0;
                std::size_t fails = 0;
                std::size_t errs  = 0;
                // OpenMP 2 compatible - MSVC not supporting higher version
#pragma omp for schedule(dynamic)
                for (long i = 0; i < tc_size; ++i)
                {
                    auto& tc = m_testcases[static_cast<std::size_t>(i)];
                    if (tc.state() == _::TestCase::State::NONE)
                    {
                        SCTF_EXEC_SILENT(m_pre_test_func)
                        tc();
                        switch (tc.state())
                        {
                            case _::TestCase::State::FAILED: ++fails; break;
                            case _::TestCase::State::ERROR: ++errs; break;
                            default: break;
                        }
                        tmp += tc.duration();
                        SCTF_EXEC_SILENT(m_post_test_func)
                        tc.set_cout(mt_buf_cout.str());
                        tc.set_cerr(mt_buf_cerr.str());
                        mt_buf_cout.clear();
                        mt_buf_cerr.clear();
                    }
                }
#pragma omp atomic
                m_stats.m_num_of_fails += fails;
#pragma omp atomic
                m_stats.m_num_of_errs += errs;
#pragma omp critical
                {
                    if (m_time < tmp)
                    {
                        m_time = tmp;
                    }
                }
            }
            m_state = State::DONE;
        }
    }

private:
    /**
     * @brief Constructor
     * @param name The name/description
     * @param context The context description
     */
    TestSuiteParallel(const std::string& name, const std::string& context)
        : TestSuite(name, context)
    {}
};

}  // namespace sctf

#endif  // SCTF_SRC_TESTSUITE_TESTSUITEPARALLEL_HPP_
