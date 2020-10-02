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

#ifndef TPP_REPORT_JSON_REPORTER_HPP
#define TPP_REPORT_JSON_REPORTER_HPP

#include <tuple>

#include "report/reporter.hpp"

#include "stringify.hpp"

namespace tpp
{
namespace intern
{
namespace report
{
/**
 * Reporter implementation with informative json output.
 */
class json_reporter : public reporter
{
public:
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
    json_reporter(enable, std::ostream& stream_) : reporter(stream_) {}

    /// Constructor for std::make_shared.
    json_reporter(enable, std::string const& fname_) : reporter(fname_) {}

private:
    void
    report_testsuite(test::testsuite_ptr const& ts_) override {
        m_first_test = true;
        conditional_prefix(m_first_suite);
        json_property_string("name", ts_->name(), color().CYAN) << ',' << newline();
        json_property_value("time", ts_->duration()) << ',' << newline();
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
    report_testcase(test::testcase const& tc_) override {
        auto const dres = decode_result(tc_.result());
        conditional_prefix(m_first_test);
        json_property_string("name", tc_.name(), color().W_BOLD) << ',' << newline();
        json_property_string("result", std::get<0>(dres), std::get<1>(dres)) << ',' << newline();
        json_property_string("reason", tc_.reason(), std::get<1>(dres)) << ',' << newline();
        json_property_value("time", tc_.duration());
        if (capture()) {
            *this << ',' << newline();
            json_property_string("stdout", tc_.cout()) << ',' << newline();
            json_property_string("stderr", tc_.cerr());
        }
        pop_indent();
        *this << newline() << '}';
        pop_indent();
    }

    void
    begin_report() override {
        reporter::begin_report();

        *this << '{';
        push_indent();
        *this << newline() << "\"testsuites\":" << space() << '[';
    }

    void
    end_report() override {
        *this << newline() << "]," << newline();
        json_property_value("count", abs_tests(), color().W_BOLD) << ',' << newline();
        json_property_value("passes", abs_tests() - faults(), color().GREEN) << ',' << newline();
        json_property_value("failures", abs_fails(), color().BLUE) << ',' << newline();
        json_property_value("errors", abs_errs(), color().RED) << ',' << newline();
        json_property_value("time", abs_time());
        pop_indent();
        *this << newline() << '}' << newline();
    }

    template<typename T>
    auto
    json_property_string(char const* name_, T&& val_, char const* col_ = nullptr) -> std::ostream& {
        return *this << '"' << name_ << "\":" << space() << (col_ ? col_ : "") << '"'
                     << escaped_string(std::forward<T>(val_)) << '"' << (col_ ? color() : "");
    }

    template<typename T>
    auto
    json_property_value(char const* name_, T&& val_, char const* col_ = nullptr) -> std::ostream& {
        return *this << '"' << name_ << "\":" << space() << (col_ ? col_ : "") << std::forward<T>(val_)
                     << (col_ ? color() : "");
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

    inline auto
    decode_result(test::testcase::results res_) -> std::tuple<char const*, char const*> {
        switch (res_) {
            case test::testcase::HAD_ERROR: return {"error", color().RED};
            case test::testcase::HAS_FAILED: return {"failure", color().BLUE};
            default: return {"success", color().GREEN};
        }
    }

    bool m_first_suite{true};
    bool m_first_test{true};
};
}  // namespace report
}  // namespace intern

using json_reporter = intern::report::json_reporter;
}  // namespace tpp

#endif  // TPP_REPORT_JSON_REPORTER_HPP
