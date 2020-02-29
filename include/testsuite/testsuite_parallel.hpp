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
 * A testsuite describes a set of tests in a certain context, like an user defined class, or
 * function. This class handles concurrently running testcases.
 */
class testsuite_parallel : public testsuite
{
public:
    testsuite_parallel(testsuite_parallel const&) = delete;
    testsuite_parallel& operator=(testsuite_parallel const&) = delete;

    virtual ~testsuite_parallel() noexcept override = default;

    /**
     * Create a new testsuite.
     * @param name_ The name/description
     * @param ctx_  The context
     * @return the newly created testsuite
     */
    static testsuite_ptr create(char const* name_)
    {
        return testsuite_ptr(new testsuite_parallel(name_));
    }

    /**
     * Execute all testcases concurrently.
     * @throws std::overflow_error if there are too many testcases for openmp to handle.
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

            SCTF_EXEC_SILENT(m_setup_fn)
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
                        SCTF_EXEC_SILENT(m_pretest_fn)
                        tc();
                        switch (tc.state())
                        {
                            case _::testcase::result::FAILED: ++fails; break;
                            case _::testcase::result::ERROR: ++errs; break;
                            default: break;
                        }
                        tmp += tc.duration();
                        SCTF_EXEC_SILENT(m_posttest_fn)
                        tc.cout(mt_buf_cout.str());
                        tc.cerr(mt_buf_cerr.str());
                        mt_buf_cout.clear();
                        mt_buf_cerr.clear();
                    }
                }
#pragma omp critical
                {
                    m_stats.m_num_of_fails += fails;
                    m_stats.m_num_of_errs += errs;
                    m_execution_time = std::max(m_execution_time, tmp);
                }
            }
            SCTF_EXEC_SILENT(m_teardown_fn)
            m_state = execution_state::DONE;
        }
    }

private:
    explicit testsuite_parallel(char const* name_) : testsuite(name_) {}
};
}  // namespace sctf

#endif  // SCTF_TESTSUITE_TESTSUITE_PARALLEL_HPP
