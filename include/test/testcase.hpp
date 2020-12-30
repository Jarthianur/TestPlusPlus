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

#ifndef TPP_TEST_TESTCASE_HPP
#define TPP_TEST_TESTCASE_HPP

#include <functional>
#include <string>
#include <utility>

#include "assert/assertion_failure.hpp"

#include "duration.hpp"

namespace tpp
{
namespace intern
{
namespace test
{
using test_function = std::function<void()>;

struct test_context final
{
    char const* const tc_name;
    char const* const ts_name;
};

class testcase
{
public:
    testcase(testcase const&) = delete;
    ~testcase() noexcept      = default;
    auto
    operator=(testcase const&) -> testcase& = delete;

    testcase(test_context&& ctx_, test_function&& fn_)
        : m_name(ctx_.tc_name), m_suite_name(ctx_.ts_name), m_test_fn(std::move(fn_)) {}

    testcase(testcase&& other_) noexcept
        : m_name(other_.m_name),
          m_suite_name(other_.m_suite_name),
          m_result(other_.m_result),
          m_elapsed_t(other_.m_elapsed_t),
          m_err_msg(std::move(other_.m_err_msg)),
          m_test_fn(std::move(other_.m_test_fn)) {}

    auto
    operator=(testcase&& other_) noexcept -> testcase& {
        m_name       = other_.m_name;
        m_suite_name = other_.m_suite_name;
        m_result     = other_.m_result;
        m_elapsed_t  = other_.m_elapsed_t;
        m_err_msg    = std::move(other_.m_err_msg);
        m_test_fn    = std::move(other_.m_test_fn);
        return *this;
    }

    enum results
    {
        IS_UNDONE,
        HAS_PASSED,
        HAS_FAILED,
        HAD_ERROR
    };

    void
    operator()() {
        if (m_result != IS_UNDONE) {
            return;
        }
        class duration dur;
        try {
            m_test_fn();
            pass();
        } catch (assert::assertion_failure const& e) {
            fail(e.what());
        } catch (std::exception const& e) {
            error(e.what());
        } catch (...) {
            error();
        }
        m_elapsed_t = dur.get();
    }

    inline auto
    result() const -> results {
        return m_result;
    }

    inline auto
    elapsed_time() const -> double {
        return m_elapsed_t;
    }

    inline auto
    reason() const -> std::string const& {
        return m_err_msg;
    }

    inline auto
    name() const -> char const* {
        return m_name;
    }

    inline auto
    suite_name() const -> char const* {
        return m_suite_name;
    }

    inline void
    cout(std::string const& str_) {
        m_cout = str_;
    }

    inline void
    cerr(std::string const& str_) {
        m_cerr = str_;
    }

    inline auto
    cout() const -> std::string const& {
        return m_cout;
    }

    inline auto
    cerr() const -> std::string const& {
        return m_cerr;
    }

private:
    inline void
    pass() {
        m_result = HAS_PASSED;
    }

    inline void
    fail(char const* msg_) {
        m_result  = HAS_FAILED;
        m_err_msg = msg_;
    }

    inline void
    error(char const* msg_ = "unknown error") {
        m_result  = HAD_ERROR;
        m_err_msg = msg_;
    }

    char const*   m_name;
    char const*   m_suite_name;
    results       m_result{IS_UNDONE};
    double        m_elapsed_t{.0};
    std::string   m_err_msg;
    std::string   m_cout;
    std::string   m_cerr;
    test_function m_test_fn;
};
}  // namespace test
}  // namespace intern
}  // namespace tpp

#endif  // TPP_TEST_TESTCASE_HPP
