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

#ifndef SCTF_REPORTER_JSON_REPORTER_HPP
#define SCTF_REPORTER_JSON_REPORTER_HPP

#include <memory>

#include "reporter/reporter.hpp"

#include "stringify.hpp"

namespace sctf
{
namespace intern
{
/**
 * Reporter implementation with informative json output.
 */
class json_reporter : public reporter
{
public:
    json_reporter(json_reporter const&)     = delete;
    json_reporter(json_reporter&&) noexcept = delete;
    ~json_reporter() noexcept override      = default;
    auto
    operator=(json_reporter const&) -> json_reporter& = delete;
    auto
    operator=(json_reporter&&) noexcept -> json_reporter& = delete;

    /**
     * Create a json reporter.
     *
     * @param stream_ is the stream where to print the report. (default: stdout)
     */
    static auto
    create(std::ostream& stream_ = std::cout) -> reporter_ptr {
        return std::make_shared<json_reporter>(enable{}, stream_);
    }

    /**
     * Create a json reporter.
     *
     * @param fname_ is the filename where to print the report.
     */
    static auto
    create(std::string const& fname_) -> reporter_ptr {
        return std::make_shared<json_reporter>(enable{}, fname_);
    }

    /// Constructor for std::make_shared.
    explicit json_reporter(enable, std::ostream& stream_) : reporter(stream_) {}

    /// Constructor for std::make_shared.
    explicit json_reporter(enable, std::string const& fname_) : reporter(fname_) {}

private:
    void
    report_testsuite(testsuite_ptr const& ts_) override {
        m_first_test = true;
        conditional_prefix(m_first_suite);
        json_property_string("name", ts_->name()) << ',' << newline();
        json_property_value("time", ts_->execution_duration()) << ',' << newline();
        json_property_value("count", ts_->statistics().tests()) << ',' << newline();
        json_property_value("passes", ts_->statistics().successes()) << ',' << newline();
        json_property_value("failures", ts_->statistics().failures()) << ',' << newline();
        json_property_value("errors", ts_->statistics().errors()) << ',' << newline();
        *this << "\"tests\":" << space() << '[';

        reporter::report_testsuite(ts_);

        *this << newline() << ']';
        pop_indent();
        *this << newline() << '}';
        pop_indent();
    }

    void
    report_testcase(testcase const& tc_) override {
        char const* result{nullptr};
        colors      col{RESET};
        switch (tc_.state()) {
            case testcase::result::ERROR:
                result = "error";
                col    = RED;
                break;
            case testcase::result::FAILED:
                result = "failure";
                col    = BLUE;
                break;
            default: result = "success"; col = GREEN;
        }
        conditional_prefix(m_first_test);
        *this << color(col);
        json_property_string("name", tc_.name()) << "," << newline();
        json_property_string("result", result) << "," << newline();
        json_property_string("reason", tc_.reason()) << "," << newline() << color();
        json_property_value("time", tc_.duration());
        if (capture()) {
            *this << "," << newline();
            json_property_string("stdout", tc_.cout()) << "," << newline();
            json_property_string("stderr", tc_.cerr());
        }
        pop_indent();
        *this << newline() << '}' << color();
        pop_indent();
    }

    void
    begin_report() override {
        reporter::begin_report();

        *this << '{';
        push_indent();
        *this << newline() << "\"testsuites\":" << space() << "[";
    }

    void
    end_report() override {
        *this << newline() << "]," << newline();
        json_property_value("count", abs_tests()) << "," << newline();
        json_property_value("passes", abs_tests() - faults()) << "," << newline();
        json_property_value("failures", abs_fails()) << "," << newline();
        json_property_value("errors", abs_errs()) << "," << newline();
        json_property_value("time", abs_time());
        pop_indent();
        *this << newline() << "}" << newline();
    }

    template<typename T>
    auto
    json_property_string(char const* name_, T&& val_) -> std::ostream& {
        return *this << '"' << name_ << "\":" << space() << '"' << escaped_string(std::forward<T>(val_)) << '"';
    }

    template<typename T>
    auto
    json_property_value(char const* name_, T&& val_) -> std::ostream& {
        return *this << '"' << name_ << "\":" << space() << std::forward<T>(val_);
    }

    void
    conditional_prefix(bool& cond_) {
        push_indent();
        if (!cond_) {
            *this << ',';
        } else {
            cond_ = false;
        }
        *this << newline() << '{';
        push_indent();
        *this << newline();
    }

    bool m_first_suite = true;
    bool m_first_test  = true;
};
}  // namespace intern

using json_reporter = intern::json_reporter;
}  // namespace sctf

#endif  // SCTF_REPORTER_JSON_REPORTER_HPP
