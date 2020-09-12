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

#ifndef SCTF_REPORTER_MARKDOWN_REPORTER_HPP
#define SCTF_REPORTER_MARKDOWN_REPORTER_HPP

#include <memory>
#include <sstream>

#include "reporter/reporter.hpp"

namespace sctf
{
namespace intern
{
/**
 * Reporter implementation with markdown format.
 */
class markdown_reporter : public reporter
{
public:
    markdown_reporter(markdown_reporter const&)     = delete;
    markdown_reporter(markdown_reporter&&) noexcept = delete;
    ~markdown_reporter() noexcept override          = default;
    auto
    operator=(markdown_reporter const&) -> markdown_reporter& = delete;
    auto
    operator=(markdown_reporter&&) noexcept -> markdown_reporter& = delete;

    /**
     * Create a markdown reporter.
     *
     * @param stream_ is the stream where to print the report. (default: stdout)
     */
    static auto
    create(std::ostream& stream_ = std::cout) -> std::shared_ptr<markdown_reporter> {
        return std::make_shared<markdown_reporter>(enable{}, stream_);
    }

    /**
     * Create a markdown reporter.
     *
     * @param fname_ is the filename where to print the report.
     */
    static auto
    create(char const* fname_) -> std::shared_ptr<markdown_reporter> {
        return std::make_shared<markdown_reporter>(enable{}, fname_);
    }

    /**
     * Enable reporting of captured output.
     *
     * @return this reporter again.
     */
    auto
    with_captured_output() -> std::shared_ptr<markdown_reporter> {
        m_capture = true;
        return std::static_pointer_cast<markdown_reporter>(shared_from_this());
    }

    /// Constructor for std::make_shared.
    explicit markdown_reporter(enable, std::ostream& stream_) : reporter(stream_) {}

    /// Constructor for std::make_shared.
    explicit markdown_reporter(enable, char const* fname_) : reporter(fname_) {}

private:
    void
    report_testsuite(testsuite_ptr const& ts_) override {
        *this << "## " << ts_->name() << fmt::XLF << "|Tests|Successes|Failures|Errors|Time|" << fmt::LF
              << "|-|-|-|-|-|" << fmt::LF << "|" << ts_->statistics().tests() << "|" << ts_->statistics().successes()
              << "|" << ts_->statistics().failures() << "|" << ts_->statistics().errors() << "|"
              << ts_->execution_duration() << "ms|" << fmt::XLF << "### Tests" << fmt::XLF
              << "|Name|Context|Time|Status|" << (m_capture ? "System-Out|System-Err|" : "") << fmt::LF << "|-|-|-|-|"
              << (m_capture ? "-|-|" : "") << fmt::LF;

        reporter::report_testsuite(ts_);

        *this << fmt::XLF;
    }

    void
    report_testcase(testcase const& tc_) override {
        char const* status = "";
        switch (tc_.state()) {
            case testcase::result::ERROR: status = "ERROR"; break;
            case testcase::result::FAILED: status = "FAILED"; break;
            case testcase::result::PASSED: status = "PASSED"; break;
            default: break;
        }
        *this << "|" << tc_.name() << "|" << tc_.suite_name() << "|" << tc_.duration() << "ms|" << status << "|";
        if (m_capture) {
            print_system_out(tc_.cout());
            print_system_out(tc_.cerr());
        }
        *this << fmt::LF;
    }

    void
    begin_report() override {
        reporter::begin_report();

        *this << "# Test Report" << fmt::XLF;
    }

    void
    end_report() override {
        *this << "## Summary" << fmt::XLF << "|Tests|Successes|Failures|Errors|Time|" << fmt::LF << "|-|-|-|-|-|"
              << fmt::LF << "|" << m_abs_tests << "|" << (m_abs_tests - m_abs_errs - m_abs_fails) << "|" << m_abs_fails
              << "|" << m_abs_errs << "|" << m_abs_time << "ms|" << fmt::LF;
    }

    void
    print_system_out(std::string const& out_) {
        std::string        line;
        std::istringstream io_;
        io_.str(out_);
        bool first = true;
        while (std::getline(io_, line)) {
            *this << (first ? "" : "<br>") << "`" << line << "`";
            first = false;
        }
        *this << "|";
    }

    bool m_capture = false;  ///< Flags whether to report captured output from testcases.
};
}  // namespace intern

using markdown_reporter = intern::markdown_reporter;
}  // namespace sctf

#endif  // SCTF_REPORTER_MARKDOWN_REPORTER_HPP
