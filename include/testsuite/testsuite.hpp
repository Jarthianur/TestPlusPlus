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
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "common/streambuf_proxy.hpp"
#include "common/stringify.hpp"
#include "common/types.hpp"
#include "testsuite/statistics.hpp"
#include "testsuite/testcase.hpp"

#define SCTF_EXEC_SILENT(F) \
    if (F)                  \
    {                       \
        try                 \
        {                   \
            F();            \
        }                   \
        catch (...)         \
        {}                  \
    }

namespace sctf
{
/**
 * A testsuite describes a set of tests in a certain context, like an user defined class, or
 * function. This class handles sequentially running testcases.
 */
class testsuite : public std::enable_shared_from_this<testsuite>
{
public:
    virtual ~testsuite() noexcept = default;

    /**
     * Create a new testsuite.
     * @param name_ The name/description
     * @param ctx_  The context
     * @return the newly created testsuite
     */
    static testsuite_ptr create(char const* name_, char const* ctx_)
    {
        return testsuite_ptr(new testsuite(name_, ctx_));
    }

    /**
     * Execute all testcases sequentially.
     */
    virtual void run()
    {
        if (m_state != execution_state::DONE)
        {
            m_stats.m_num_of_tests = m_testcases.size();
            _::streambuf_proxy buf_cout(std::cout);
            _::streambuf_proxy buf_cerr(std::cerr);

            SCTF_EXEC_SILENT(m_setup_fn)
            std::for_each(m_testcases.begin(), m_testcases.end(),
                          [this, &buf_cerr, &buf_cout](_::testcase& tc_) {
                              if (tc_.state() == _::testcase::result::NONE)
                              {
                                  SCTF_EXEC_SILENT(m_pretest_fn)
                                  tc_();
                                  switch (tc_.state())
                                  {
                                      case _::testcase::result::FAILED:
                                          ++m_stats.m_num_of_fails;
                                          break;
                                      case _::testcase::result::ERROR:
                                          ++m_stats.m_num_of_errs;
                                          break;
                                      default: break;
                                  }
                                  m_time += tc_.duration();
                                  SCTF_EXEC_SILENT(m_posttest_fn)
                                  tc_.cout(buf_cout.str());
                                  tc_.cerr(buf_cerr.str());
                                  buf_cout.clear();
                                  buf_cerr.clear();
                              }
                          });
            SCTF_EXEC_SILENT(m_teardown_fn)
            m_state = execution_state::DONE;
        }
    }

    /**
     * Add a new testcase to this testsuite.
     * @tparam T The class context
     * @param name_ The name/description
     * @param fn_   The test function
     * @return this testsuite for chaining
     */
    template<typename T>
    testsuite_ptr test(char const* name_, _::test_function&& fn_)
    {
        m_testcases.push_back(_::testcase(name_, _::name_for_type<T>(), std::move(fn_)));
        m_state = execution_state::PENDING;
        return shared_from_this();
    }

    /**
     * Add a new testcase to this testsuite.
     * @param name_ The name/description
     * @param ctx_  The context
     * @param fn_   The test function
     * @return this testsuite for chaining
     */
    testsuite_ptr test(char const* name_, char const* ctx_, _::test_function&& fn_)
    {
        m_testcases.push_back(_::testcase(name_, ctx_, std::move(fn_)));
        m_state = execution_state::PENDING;
        return shared_from_this();
    }

    /**
     * Add a new testcase to this testsuite, where the context of the test is inherited from the
     * suite.
     * @param name_ The name/description
     * @param fn_   The test function
     * @return this testsuite for chaining
     */
    testsuite_ptr test(char const* name_, _::test_function&& fn_)
    {
        m_testcases.push_back(_::testcase(name_, m_context, std::move(fn_)));
        m_state = execution_state::PENDING;
        return shared_from_this();
    }

    /**
     * Set a function, which will be executed once before all testcases.
     * Overwrites the old setup-function, if any was set yet.
     * Exceptions thrown by the function will be ignored.
     * @param fn_ The function
     * @return this testsuite for chaining
     */
    testsuite_ptr setup(_::test_function&& fn_)
    {
        m_setup_fn = std::move(fn_);
        return shared_from_this();
    }

    /**
     * Set a function, which will be executed once after all testcases.
     * Overwrites the old teardown-function, if any was set yet.
     * Exceptions thrown by the function will be ignored.
     * @param fn_ The function
     * @return this testsuite for chaining
     */
    testsuite_ptr teardown(_::test_function&& fn_)
    {
        m_teardown_fn = std::move(fn_);
        return shared_from_this();
    }

    /**
     * Set a function, which will be executed before each testcase.
     * Overwrites the old before-function, if any was set yet.
     * Exceptions thrown by the function will be ignored.
     * @param fn_ The function
     * @return this testsuite for chaining
     */
    testsuite_ptr before_each(_::test_function&& fn_)
    {
        m_pretest_fn = std::move(fn_);
        return shared_from_this();
    }

    /**
     * Set a function, which will be executed after each testcase.
     * Overwrites the old after-function, if any was set yet.
     * Exceptions thrown by the function will be ignored.
     * @param fn_ The function
     * @return this testsuite for chaining
     */
    testsuite_ptr after_each(_::test_function&& fn_)
    {
        m_posttest_fn = std::move(fn_);
        return shared_from_this();
    }

    /**
     * Get the testsuite name.
     */
    inline char const* name() const
    {
        return m_name;
    }

    /**
     * Get the testsuite timestamp of instantiation.
     */
    inline std::chrono::system_clock::time_point const& timestamp() const
    {
        return m_timestamp;
    }

    /**
     * Get the test statistics.
     */
    inline _::statistics const& statistics() const
    {
        return m_stats;
    }

    /**
     * Get the accumulated time spent on all tests.
     */
    inline double time() const
    {
        return m_time;
    }

    /**
     * Get all testcases.
     */
    inline std::vector<_::testcase> const& testcases() const
    {
        return m_testcases;
    }

protected:
    enum class execution_state : std::int_fast8_t
    {
        PENDING,
        DONE
    };

    testsuite(char const* name_, char const* ctx_)
        : m_name(name_), m_context(ctx_), m_timestamp(std::chrono::system_clock::now())
    {}

    char const*                                 m_name;
    char const*                                 m_context;
    std::chrono::system_clock::time_point const m_timestamp;
    double                                      m_time = 0.0;

    _::statistics            m_stats;
    std::vector<_::testcase> m_testcases;
    execution_state          m_state = execution_state::PENDING;

    _::test_function m_setup_fn;
    _::test_function m_teardown_fn;
    _::test_function m_pretest_fn;
    _::test_function m_posttest_fn;
};

}  // namespace sctf

#endif  // SCTF_TESTSUITE_TESTSUITE_HPP
