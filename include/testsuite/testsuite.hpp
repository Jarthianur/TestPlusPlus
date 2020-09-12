/*
    Copyright (C) 2017 Jarthianur

    This file is part of simple-cpp-test-framework.

    simple-cpp-test-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple-cpp-test-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple-cpp-test-framework.  If not, see <https://www.gnu.org/licenses/>.
*/

/// @file

#ifndef SCTF_TESTSUITE_TESTSUITE_HPP
#define SCTF_TESTSUITE_TESTSUITE_HPP

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "testsuite/statistic.hpp"
#include "testsuite/streambuf_proxy.hpp"
#include "testsuite/testcase.hpp"

namespace sctf
{
namespace intern
{
class testsuite;
using testsuite_ptr = std::shared_ptr<testsuite>;

/**
 * Group testcases together in a testsuite. Testcases share this testsuite as their context.
 * Testcases are run sequentially.
 */
class testsuite
{
protected:
    /// Helper type to prevent public constructor usage.
    struct enable
    {};

public:
    using hook_function = std::function<void()>;

    testsuite(testsuite const&)     = delete;
    testsuite(testsuite&&) noexcept = delete;
    virtual ~testsuite() noexcept   = default;
    auto
    operator=(testsuite const&) -> testsuite& = delete;
    auto
    operator=(testsuite&&) noexcept -> testsuite& = delete;

    /**
     * Create a new testsuite.
     *
     * @param name_ is the name, or description of the testsuite.
     */
    static auto
    create(char const* name_) -> testsuite_ptr {
        return std::make_shared<testsuite>(enable{}, name_);
    }

    /**
     * Run all testcases in this suite.
     */
    virtual void
    run() {
        if (m_state != execution_state::DONE) {
            m_stats.m_num_tests = m_testcases.size();
            streambuf_proxy buf_cout(std::cout);
            streambuf_proxy buf_cerr(std::cerr);
            m_setup_fn();
            std::for_each(m_testcases.begin(), m_testcases.end(), [this, &buf_cerr, &buf_cout](testcase& tc_) {
                if (tc_.state() == testcase::result::NONE) {
                    m_pretest_fn();
                    tc_();
                    switch (tc_.state()) {
                        case testcase::result::FAILED: ++m_stats.m_num_fails; break;
                        case testcase::result::ERROR: ++m_stats.m_num_errs; break;
                        default: break;
                    }
                    m_exec_dur += tc_.duration();
                    m_posttest_fn();
                    tc_.cout(buf_cout.str());
                    tc_.cerr(buf_cerr.str());
                    buf_cout.clear();
                    buf_cerr.clear();
                }
            });
            m_teardown_fn();
            m_state = execution_state::DONE;
        }
    }

    /**
     * Add a new testcase to this testsuite.
     *
     * @param name_ is the name, or description of the testcase.
     * @param fn_   is the function performing the test.
     */
    void
    test(char const* name_, hook_function&& fn_) {
        m_testcases.emplace_back(test_context{name_, m_name}, std::move(fn_));
        m_state = execution_state::PENDING;
    }

    /**
     * Set a function, which will be executed once before all testcases.
     * Exceptions thrown by the function will be ignored.
     *
     * @param fn_ is the function to set.
     */
    void
    setup(hook_function&& fn_) {
        m_setup_fn.fn = std::move(fn_);
    }

    /**
     * Set a function, which will be executed once after all testcases.
     * Exceptions thrown by the function will be ignored.
     *
     * @param fn_ is the function to set.
     */
    void
    teardown(hook_function&& fn_) {
        m_teardown_fn.fn = std::move(fn_);
    }

    /**
     * Set a function, which will be executed before each testcase.
     * Exceptions thrown by the function will be ignored.
     *
     * @param fn_ is the function to set.
     */
    void
    before_each(hook_function&& fn_) {
        m_pretest_fn.fn = std::move(fn_);
    }

    /**
     * Set a function, which will be executed after each testcase.
     * Exceptions thrown by the function will be ignored.
     *
     * @param fn_ is the function to set.
     */
    void
    after_each(hook_function&& fn_) {
        m_posttest_fn.fn = std::move(fn_);
    }

    /**
     * Get the testsuite name.
     */
    inline auto
    name() const -> char const* {
        return m_name;
    }

    /**
     * Get the timestamp of instantiation.
     */
    inline auto
    timestamp() const -> std::chrono::system_clock::time_point const& {
        return m_create_time;
    }

    /**
     * Get the test statistics.
     */
    inline auto
    statistics() const -> statistic const& {
        return m_stats;
    }

    /**
     * Get the accumulated time spent on all tests.
     */
    inline auto
    execution_duration() const -> double {
        return m_exec_dur;
    }

    /**
     * Get all testcases.
     */
    inline auto
    testcases() const -> std::vector<testcase> const& {
        return m_testcases;
    }

    /**
     * Constructor for std::make_shared.
     *
     * @param name_ is the name, or description of the testsuite.
     */
    explicit testsuite(enable, char const* name_) : m_name(name_), m_create_time(std::chrono::system_clock::now()) {}

protected:
    /**
     * Hold a function optionally, that can be executed without throwing any exception.
     */
    struct silent_functor final
    {
        auto
        operator()() -> silent_functor& {
            if (fn) {
                try {
                    fn();
                } catch (...) {
                }
            }
            return *this;
        }

        hook_function fn;
    };

    /**
     * State whether there are testcases, which not have been run.
     */
    enum class execution_state : std::int_fast8_t
    {
        PENDING,  /// At least one testcase has not been run.
        DONE      /// All testcases have been run.
    };

    char const*                                 m_name;         ///< Name, or description of this testsuite.
    std::chrono::system_clock::time_point const m_create_time;  ///< Point in time, when this testsuite was created.
    double m_exec_dur = 0.0;  ///< Time in milliseconds, the whole testsuite needed to complete.

    statistic             m_stats;                             ///< Stores test results.
    std::vector<testcase> m_testcases;                         ///< Stores testcases.
    execution_state       m_state = execution_state::PENDING;  ///< States, whether all testcases have been executed.

    silent_functor m_setup_fn;     ///< Optional function, that is executed before all testcases.
    silent_functor m_teardown_fn;  ///< Optional function, that is executed after all testcases.
    silent_functor m_pretest_fn;   ///< Optional function, that is executed before all testcases.
    silent_functor m_posttest_fn;  ///< Optional function, that is executed before all testcases.
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_TESTSUITE_TESTSUITE_HPP
