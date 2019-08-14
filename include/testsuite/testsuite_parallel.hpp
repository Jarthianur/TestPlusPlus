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

#ifndef SCTF_TESTSUITE_TESTSUITE_PARALLEL_HPP
#define SCTF_TESTSUITE_TESTSUITE_PARALLEL_HPP

#include <limits>
#include <stdexcept>

#include "common/streambuf_proxy_omp.hpp"
#include "testsuite/testsuite.hpp"

namespace sctf
{
/**
 * @brief Testsuite class for managing parallel testcases.
 * @note Non-copyable
 */
class testsuite_parallel : public testsuite
{
public:
    ~testsuite_parallel() noexcept override = default;

    /**
     * @brief Create a TestSuiteParallel.
     * @param name The name/description
     * @param context The context
     * @return a shared pointer to the created TestSuiteParallel
     */
    static testsuite_shared create(const char* name, const char* context)
    {
        return testsuite_shared(new testsuite_parallel(name, context));
    }

    /**
     * @brief Execute all TestCases in parallel.
     */
    void run() override
    {
        if (m_state != execution_state::DONE)
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
                    if (tc.state() == _::testcase::result::NONE)
                    {
                        SCTF_EXEC_SILENT(m_pre_test_func)
                        tc();
                        switch (tc.state())
                        {
                            case _::testcase::result::FAILED: ++fails; break;
                            case _::testcase::result::ERROR: ++errs; break;
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
#pragma omp critical
                {
                    m_stats.m_num_of_fails += fails;
                    m_stats.m_num_of_errs += errs;
                    m_time = std::max(m_time, tmp);
                }
            }
            m_state = execution_state::DONE;
        }
    }

private:
    /**
     * @brief Constructor
     * @param name The name/description
     * @param context The context description
     */
    testsuite_parallel(const char* name, const char* context) : testsuite(name, context) {}
};

}  // namespace sctf

#endif  // SCTF_TESTSUITE_TESTSUITE_PARALLEL_HPP
