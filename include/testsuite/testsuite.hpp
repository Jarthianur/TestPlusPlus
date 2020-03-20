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

#ifndef SCTF_TESTSUITE_TESTSUITE_HPP
#define SCTF_TESTSUITE_TESTSUITE_HPP

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "testsuite/statistic.hpp"
#include "testsuite/streambuf_proxy.hpp"

#include "stringify.hpp"
#include "testcase.hpp"

#define SCTF_PRIVATE_EXEC_SILENT(F) \
    if (F) {                        \
        try {                       \
            F();                    \
        } catch (...) {             \
        }                           \
    }

namespace sctf
{
namespace private_
{
class testsuite;
using testsuite_ptr = std::shared_ptr<testsuite>;

/**
 * A testsuite describes a set of tests in a certain context, like an user defined class, or
 * function. This class handles sequentially running testcases.
 */
class testsuite
{
public:
    testsuite(testsuite const&) = delete;
    testsuite& operator=(testsuite const&) = delete;

    virtual ~testsuite() noexcept = default;

    /**
     * Create a new testsuite.
     * @param name_ The name/description
     * @return the newly created testsuite
     */
    static testsuite_ptr create(char const* name_) {
        return testsuite_ptr(new testsuite(name_));
    }

    /**
     * Execute all testcases sequentially.
     */
    virtual void run() {
        if (m_state != execution_state::DONE) {
            SCTF_PRIVATE_EXEC_SILENT(m_setup_fn)
            m_stats.m_num_of_tests = m_testcases.size();
            streambuf_proxy buf_cout(std::cout);
            streambuf_proxy buf_cerr(std::cerr);

            std::for_each(m_testcases.begin(), m_testcases.end(),
                          [this, &buf_cerr, &buf_cout](testcase& tc_) {
                              if (tc_.state() == testcase::result::NONE) {
                                  SCTF_PRIVATE_EXEC_SILENT(m_pretest_fn)
                                  tc_();
                                  switch (tc_.state()) {
                                      case testcase::result::FAILED:
                                          ++m_stats.m_num_of_fails;
                                          break;
                                      case testcase::result::ERROR: ++m_stats.m_num_of_errs; break;
                                      default: break;
                                  }
                                  m_execution_time += tc_.duration();
                                  SCTF_PRIVATE_EXEC_SILENT(m_posttest_fn)
                                  tc_.cout(buf_cout.str());
                                  tc_.cerr(buf_cerr.str());
                                  buf_cout.clear();
                                  buf_cerr.clear();
                              }
                          });
            SCTF_PRIVATE_EXEC_SILENT(m_teardown_fn)
            m_state = execution_state::DONE;
        }
    }

    /**
     * Add a new testcase to this testsuite, where the context of the test is inherited from the
     * suite.
     * @param name_ The name/description
     * @param fn_   The test function
     * @return this testsuite for chaining
     */
    void test(char const* name_, void_function&& fn_) {
        m_testcases.push_back(testcase(name_, m_name, std::move(fn_)));
        m_state = execution_state::PENDING;
    }

    /**
     * Set a function, which will be executed once before all testcases.
     * Overwrites the old setup-function, if any was set yet.
     * Exceptions thrown by the function will be ignored.
     * @param fn_ The function
     * @return this testsuite for chaining
     */
    void setup(void_function&& fn_) {
        m_setup_fn = std::move(fn_);
    }

    /**
     * Set a function, which will be executed once after all testcases.
     * Overwrites the old teardown-function, if any was set yet.
     * Exceptions thrown by the function will be ignored.
     * @param fn_ The function
     * @return this testsuite for chaining
     */
    void teardown(void_function&& fn_) {
        m_teardown_fn = std::move(fn_);
    }

    /**
     * Set a function, which will be executed before each testcase.
     * Overwrites the old before-function, if any was set yet.
     * Exceptions thrown by the function will be ignored.
     * @param fn_ The function
     * @return this testsuite for chaining
     */
    void before_each(void_function&& fn_) {
        m_pretest_fn = std::move(fn_);
    }

    /**
     * Set a function, which will be executed after each testcase.
     * Overwrites the old after-function, if any was set yet.
     * Exceptions thrown by the function will be ignored.
     * @param fn_ The function
     * @return this testsuite for chaining
     */
    void after_each(void_function&& fn_) {
        m_posttest_fn = std::move(fn_);
    }

    /**
     * Get the testsuite name.
     */
    inline char const* name() const {
        return m_name;
    }

    /**
     * Get the testsuite timestamp of instantiation.
     */
    inline std::chrono::system_clock::time_point const& timestamp() const {
        return m_timestamp;
    }

    /**
     * Get the test statistics.
     */
    inline statistic const& statistics() const {
        return m_stats;
    }

    /**
     * Get the accumulated time spent on all tests.
     */
    inline double execution_time() const {
        return m_execution_time;
    }

    /**
     * Get all testcases.
     */
    inline std::vector<testcase> const& testcases() const {
        return m_testcases;
    }

protected:
    enum class execution_state : std::int_fast8_t
    {
        PENDING,
        DONE
    };

    explicit testsuite(char const* name_)
        : m_name(name_), m_timestamp(std::chrono::system_clock::now()) {}

    char const*                                 m_name;
    std::chrono::system_clock::time_point const m_timestamp;
    double                                      m_execution_time = 0.0;

    statistic             m_stats;
    std::vector<testcase> m_testcases;
    execution_state       m_state = execution_state::PENDING;

    void_function m_setup_fn;
    void_function m_teardown_fn;
    void_function m_pretest_fn;
    void_function m_posttest_fn;
};

}  // namespace private_
}  // namespace sctf

#endif  // SCTF_TESTSUITE_TESTSUITE_HPP
