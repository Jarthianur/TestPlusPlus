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

#ifndef TPP_REPORT_CONSOLE_REPORTER_HPP
#define TPP_REPORT_CONSOLE_REPORTER_HPP

#include <memory>

#include "report/reporter.hpp"

namespace tpp
{
namespace intern
{
namespace report
{
/**
 * Reporter implementation with informative console output.
 */
class console_reporter : public reporter
{
public:
    console_reporter(console_reporter const&)     = delete;
    console_reporter(console_reporter&&) noexcept = delete;
    ~console_reporter() noexcept override         = default;
    auto
    operator=(console_reporter const&) -> console_reporter& = delete;
    auto
    operator=(console_reporter&&) noexcept -> console_reporter& = delete;

    /**
     * Create a console reporter.
     *
     * @param stream_ is the stream where to print the report. (default: stdout)
     */
    static auto
    create(std::ostream& stream_ = std::cout) -> reporter_ptr {
        return std::make_shared<console_reporter>(enable{}, stream_);
    }

    /**
     * Create a console reporter.
     *
     * @param fname_ is the filename where to print the report.
     */
    static auto
    create(std::string const& fname_) -> reporter_ptr {
        return std::make_shared<console_reporter>(enable{}, fname_);
    }

    /// Constructor for std::make_shared.
    console_reporter(enable, std::ostream& stream_) : reporter(stream_) {}

    /// Constructor for std::make_shared.
    console_reporter(enable, std::string const& fname_) : reporter(fname_) {}

private:
    void
    report_testsuite(test::testsuite_ptr const& ts_) override {
        *this << "--- " << ts_->name() << " (" << ts_->duration() << "ms) ---" << fmt::LF;

        reporter::report_testsuite(ts_);
        *this << fmt::LF;
    }

    void
    report_testcase(test::testcase const& tc_) override {
        *this << fmt::SPACE << tc_.name() << " (" << tc_.duration() << "ms)" << fmt::LF << fmt::SPACE << fmt::SPACE;
        if (capture()) {
            *this << "stdout = \"" << tc_.cout() << "\"" << fmt::LF << fmt::SPACE << fmt::SPACE;
            *this << "stderr = \"" << tc_.cerr() << "\"" << fmt::LF << fmt::SPACE << fmt::SPACE;
        }
        switch (tc_.result()) {
            case test::testcase::HAS_ERROR: *this << color(RED) << "ERROR! " << tc_.reason(); break;
            case test::testcase::HAS_FAILED: *this << color(BLUE) << "FAILED! " << tc_.reason(); break;
            default: *this << color(GREEN) << "PASSED!"; break;
        }
        *this << color() << fmt::LF;
    }

    void
    end_report() override {
        if (abs_errs() > 0) {
            *this << color(YELLOW);
        } else if (abs_fails() > 0) {
            *this << color(BLUE);
        } else {
            *this << color(CYAN);
        }
        *this << "=== Result ===" << fmt::LF << "passes: " << abs_tests() - faults() << "/" << abs_tests()
              << " failures: " << abs_fails() << "/" << abs_tests() << " errors: " << abs_errs() << "/" << abs_tests()
              << " (" << abs_time() << "ms)" << color() << fmt::LF;
    }
};
}  // namespace report
}  // namespace intern

using console_reporter = intern::report::console_reporter;
}  // namespace tpp

#endif  // TPP_REPORT_CONSOLE_REPORTER_HPP
