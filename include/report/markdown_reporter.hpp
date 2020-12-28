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

#ifndef TPP_REPORT_MARKDOWN_REPORTER_HPP
#define TPP_REPORT_MARKDOWN_REPORTER_HPP

#include <sstream>

#include "report/reporter.hpp"

namespace tpp
{
namespace intern
{
namespace report
{
/**
 * Reporter implementation with markdown format.
 */
class markdown_reporter : public reporter
{
public:
    /**
     * Create a markdown reporter.
     *
     * @param stream_ is the stream where to print the report. (default: stdout)
     */
    static auto
    create(std::ostream& stream_ = std::cout) -> reporter_ptr {
        return std::make_shared<markdown_reporter>(enable{}, stream_);
    }

    /**
     * Create a markdown reporter.
     *
     * @param fname_ is the filename where to print the report.
     */
    static auto
    create(std::string const& fname_) -> reporter_ptr {
        return std::make_shared<markdown_reporter>(enable{}, fname_);
    }

    /// Constructor for std::make_shared.
    markdown_reporter(enable, std::ostream& stream_) : reporter(stream_) {}

    /// Constructor for std::make_shared.
    markdown_reporter(enable, std::string const& fname_) : reporter(fname_) {}

private:
    void
    report_testsuite(test::testsuite_ptr const& ts_) override {
        *this << "## " << ts_->name() << fmt::LF << fmt::LF << "|Tests|Successes|Failures|Errors|Time|" << fmt::LF
              << "|-|-|-|-|-|" << fmt::LF << '|' << ts_->statistics().tests() << '|' << ts_->statistics().successes()
              << '|' << ts_->statistics().failures() << '|' << ts_->statistics().errors() << '|'
              << ts_->statistics().elapsed_time() << "ms|" << fmt::LF << fmt::LF << "### Tests" << fmt::LF << fmt::LF
              << "|Name|Time|Status|" << fmt::LF << "|-|-|-|" << fmt::LF;

        reporter::report_testsuite(ts_);

        *this << fmt::LF;
        std::for_each(ts_->testcases().cbegin(), ts_->testcases().cend(),
                      [this](test::testcase const& tc_) { testcase_details(tc_); });
    }

    void
    report_testcase(test::testcase const& tc_) override {
        auto const status = [&] {
            switch (tc_.result()) {
                case test::testcase::HAD_ERROR: return "ERROR";
                case test::testcase::HAS_FAILED: return "FAILED";
                default: return "PASSED";
            }
        };
        *this << '|' << tc_.name() << '|' << tc_.elapsed_time() << "ms|" << status() << '|' << fmt::LF;
    }

    void
    begin_report() override {
        reporter::begin_report();

        *this << "# Test Report" << fmt::LF << fmt::LF;
    }

    void
    end_report() override {
        *this << "## Summary" << fmt::LF << fmt::LF << "|Tests|Successes|Failures|Errors|Time|" << fmt::LF
              << "|-|-|-|-|-|" << fmt::LF << '|' << abs_tests() << '|' << (abs_tests() - faults()) << '|' << abs_fails()
              << '|' << abs_errs() << '|' << abs_time() << "ms|" << fmt::LF;
    }

    void
    testcase_details(test::testcase const& tc_) {
        bool const isFailed = tc_.result() != test::testcase::HAS_PASSED;
        if (isFailed || capture()) {
            *this << "#### " << tc_.name() << fmt::LF << fmt::LF;
        }
        if (isFailed) {
            *this << "##### Reason" << fmt::LF << fmt::LF << tc_.reason() << fmt::LF << fmt::LF;
        }
        if (capture()) {
            print_system_out("Out", tc_.cout());
            print_system_out("Err", tc_.cerr());
        }
    }

    void
    print_system_out(char const* chan_, std::string const& out_) {
        *this << "##### System-" << chan_ << fmt::LF << fmt::LF;
        if (!out_.empty()) {
            *this << "```" << fmt::LF << out_ << fmt::LF << "```" << fmt::LF << fmt::LF;
        }
    }
};
}  // namespace report
}  // namespace intern

using markdown_reporter = intern::report::markdown_reporter;
}  // namespace tpp

#endif  // TPP_REPORT_MARKDOWN_REPORTER_HPP
