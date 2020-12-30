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

#ifndef TPP_TEST_TESTSUITE_HPP
#define TPP_TEST_TESTSUITE_HPP

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "test/statistic.hpp"
#include "test/streambuf_proxy.hpp"
#include "test/testcase.hpp"

namespace tpp
{
namespace intern
{
namespace test
{
template<typename T>
struct streambuf_proxies
{
    T cout{std::cout};
    T cerr{std::cerr};
};

class testsuite;
using testsuite_ptr = std::shared_ptr<testsuite>;

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

    static auto
    create(char const* name_) -> testsuite_ptr {
        return std::make_shared<testsuite>(enable{}, name_);
    }

    virtual void
    run() {
        if (m_state != IS_DONE) {
            duration d;
            m_stats.m_num_tests = m_testcases.size();
            streambuf_proxies<streambuf_proxy_single> bufs;
            m_setup_fn();
            std::for_each(m_testcases.begin(), m_testcases.end(), [&](testcase& tc_) {
                if (tc_.result() == testcase::IS_UNDONE) {
                    m_pretest_fn();
                    tc_();
                    switch (tc_.result()) {
                        case testcase::HAS_FAILED: ++m_stats.m_num_fails; break;
                        case testcase::HAD_ERROR: ++m_stats.m_num_errs; break;
                        default: break;
                    }
                    m_posttest_fn();
                    tc_.cout(bufs.cout.str());
                    tc_.cerr(bufs.cerr.str());
                }
            });
            m_teardown_fn();
            m_state = IS_DONE;
            m_stats.m_elapsed_t += d.get();
        }
    }

    void
    test(char const* name_, hook_function&& fn_) {
        m_testcases.emplace_back(test_context{name_, m_name}, std::move(fn_));
        m_state = IS_PENDING;
    }

    void
    setup(hook_function&& fn_) {
        m_setup_fn.fn = std::move(fn_);
    }

    void
    teardown(hook_function&& fn_) {
        m_teardown_fn.fn = std::move(fn_);
    }

    void
    before_each(hook_function&& fn_) {
        m_pretest_fn.fn = std::move(fn_);
    }

    void
    after_each(hook_function&& fn_) {
        m_posttest_fn.fn = std::move(fn_);
    }

    inline auto
    name() const -> char const* {
        return m_name;
    }

    inline auto
    timestamp() const -> std::chrono::system_clock::time_point const& {
        return m_create_time;
    }

    inline auto
    statistics() const -> statistic const& {
        return m_stats;
    }

    inline auto
    testcases() const -> std::vector<testcase> const& {
        return m_testcases;
    }

    testsuite(enable, char const* name_) : m_name(name_), m_create_time(std::chrono::system_clock::now()) {}

protected:
    struct optional_functor final
    {
        void
        operator()() const noexcept {
            if (fn) {
                fn();
            }
        }

        hook_function fn;
    };

    enum states
    {
        IS_PENDING,
        IS_DONE
    };

    char const* const                           m_name;
    std::chrono::system_clock::time_point const m_create_time;

    statistic             m_stats;
    std::vector<testcase> m_testcases;
    states                m_state{IS_PENDING};

    optional_functor m_setup_fn;
    optional_functor m_teardown_fn;
    optional_functor m_pretest_fn;
    optional_functor m_posttest_fn;
};
}  // namespace test
}  // namespace intern
}  // namespace tpp

#endif  // TPP_TEST_TESTSUITE_HPP
