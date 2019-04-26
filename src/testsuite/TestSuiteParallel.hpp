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

#ifdef _OPENMP
#    include <omp.h>
#endif

#include "../util/mt_streambuf.hpp"

#include "TestSuite.hpp"

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

    /**
     * @brief Destructor
     */
    ~TestSuiteParallel() noexcept = default;

    /**
     * @brief Execute all TestCases in parallel.
     */
    void run() noexcept override
    {
        if (m_state == State::DONE) return;
        m_stats.m_num_of_tests = m_testcases.size();
        util::mt_streambuf mt_buf(std::cout);
        SCTF_EXEC_SILENT(m_setup_func)
#pragma omp parallel
        {
            double      tmp   = 0.0;
            std::size_t fails = 0;
            std::size_t errs  = 0;
#pragma omp for schedule(dynamic)
            for (auto tc = m_testcases.begin(); tc < m_testcases.end(); ++tc)
            {
                if (tc->state() != test::TestCase::State::NONE) continue;
                SCTF_EXEC_SILENT(m_pre_test_func)
                (*tc)();
                switch (tc->state())
                {
                    case test::TestCase::State::FAILED: ++fails; break;
                    case test::TestCase::State::ERROR: ++errs; break;
                    default: break;
                }
                tmp += tc->duration();
                SCTF_EXEC_SILENT(m_post_test_func)
                tc->set_cout(mt_buf.get_buf());
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
