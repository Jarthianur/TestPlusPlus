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

#include "testsuite/streambuf_proxy_omp.hpp"
#include "testsuite/testsuite.hpp"

namespace sctf
{
namespace intern
{
/**
 * Group testcases together in a testsuite. Testcases share this testsuite as their context.
 * Testcases are run in parallel.
 */
class testsuite_parallel : public testsuite
{
public:
    testsuite_parallel(testsuite_parallel const&) = delete;
    testsuite_parallel& operator=(testsuite_parallel const&) = delete;

    virtual ~testsuite_parallel() noexcept override = default;

    /**
     * Create a new testsuite.
     *
     * @param name_ is the name, or description of the testsuite.
     */
    static testsuite_ptr create(char const* name_) {
        return testsuite_ptr(new testsuite_parallel(name_));
    }

    /**
     * Run all testcases in this suite.
     * @throw std::overflow_error if there are too many testcases for openmp to handle.
     */
    void run() override {
        if (m_state != execution_state::DONE) {
            if (m_testcases.size() > static_cast<std::size_t>(std::numeric_limits<long>::max())) {
                throw std::overflow_error("Too many testcases! Size would overflow loop variant.");
            }
            m_setup_fn();
            const long tc_size     = static_cast<long>(m_testcases.size());
            m_stats.m_num_of_tests = m_testcases.size();
            streambuf_proxy_omp mt_buf_cout(std::cout);
            streambuf_proxy_omp mt_buf_cerr(std::cerr);
#pragma omp parallel
            {  // BEGIN parallel section
                double      tmp   = 0.0;
                std::size_t fails = 0;
                std::size_t errs  = 0;
                // OpenMP 2 compatible - MSVC not supporting higher version
#pragma omp for schedule(dynamic)
                for (long i = 0; i < tc_size; ++i) {
                    auto& tc = m_testcases[static_cast<std::size_t>(i)];
                    if (tc.state() == testcase::result::NONE) {
                        m_pretest_fn();
                        tc();
                        switch (tc.state()) {
                            case testcase::result::FAILED: ++fails; break;
                            case testcase::result::ERROR: ++errs; break;
                            default: break;
                        }
                        tmp += tc.duration();
                        m_posttest_fn();
                        tc.cout(mt_buf_cout.str());
                        tc.cerr(mt_buf_cerr.str());
                        mt_buf_cout.clear();
                        mt_buf_cerr.clear();
                    }
                }
#pragma omp critical
                {  // BEGIN critical section
                    m_stats.m_num_of_fails += fails;
                    m_stats.m_num_of_errs += errs;
                    m_exec_dur = std::max(m_exec_dur, tmp);
                }  // END critical section
            }      // END parallel section
            m_teardown_fn();
            m_state = execution_state::DONE;
        }
    }

private:
    /**
     * @param name_ is the name, or description of the testsuite.
     */
    explicit testsuite_parallel(char const* name_) : testsuite(name_) {}
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_TESTSUITE_TESTSUITE_PARALLEL_HPP
