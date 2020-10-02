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

#include <regex>
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
              << '|' << ts_->statistics().failures() << '|' << ts_->statistics().errors() << '|' << ts_->duration()
              << "ms|" << fmt::LF << fmt::LF << "### Tests" << fmt::LF << fmt::LF << "|Name|Context|Time|Status|"
              << (capture() ? "System-Out|System-Err|" : "") << fmt::LF << "|-|-|-|-|" << (capture() ? "-|-|" : "")
              << fmt::LF;

        reporter::report_testsuite(ts_);

        *this << fmt::LF << fmt::LF;
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
        *this << '|' << tc_.name() << '|' << tc_.suite_name() << '|' << tc_.duration() << "ms|" << status() << '|';
        if (capture()) {
            print_system_out(tc_.cout());
            print_system_out(tc_.cerr());
        }
        *this << fmt::LF;
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
    print_system_out(std::string const& out_) {
        std::string        line;
        std::istringstream io_;
        io_.str(out_);
        bool first{true};
        while (std::getline(io_, line)) {
            *this << (first ? "" : "<br>") << '`' << std::regex_replace(line, std::regex("`"), "``") << '`';
            first = false;
        }
        *this << '|';
    }
};
}  // namespace report
}  // namespace intern

using markdown_reporter = intern::report::markdown_reporter;
}  // namespace tpp

#endif  // TPP_REPORT_MARKDOWN_REPORTER_HPP
