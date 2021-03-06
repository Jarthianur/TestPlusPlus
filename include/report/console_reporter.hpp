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

#include "report/reporter.hpp"

#include "stringify.hpp"

namespace tpp
{
namespace intern
{
namespace report
{
class console_reporter : public reporter
{
public:
    static auto
    create(std::ostream& stream_ = std::cout) -> reporter_ptr {
        return std::make_shared<console_reporter>(enable{}, stream_);
    }

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
        *this << "--- " << ts_->name() << " (" << ts_->statistics().elapsed_time() << "ms) ---" << fmt::LF;

        reporter::report_testsuite(ts_);
        *this << fmt::LF;
    }

    void
    report_testcase(test::testcase const& tc_) override {
        *this << fmt::SPACE << tc_.name() << " (" << tc_.elapsed_time() << "ms)" << fmt::LF << fmt::SPACE << fmt::SPACE;
        if (capture()) {
            *this << "stdout = \"" << escaped_string(tc_.cout()) << '"' << fmt::LF << fmt::SPACE << fmt::SPACE;
            *this << "stderr = \"" << escaped_string(tc_.cerr()) << '"' << fmt::LF << fmt::SPACE << fmt::SPACE;
        }
        switch (tc_.result()) {
            case test::testcase::HAD_ERROR: *this << color().RED << "ERROR! " << tc_.reason(); break;
            case test::testcase::HAS_FAILED: *this << color().BLUE << "FAILED! " << tc_.reason(); break;
            default: *this << color().GREEN << "PASSED!"; break;
        }
        *this << color() << fmt::LF;
    }

    void
    end_report() override {
        if (abs_errs() > 0) {
            *this << color().MAGENTA;
        } else if (abs_fails() > 0) {
            *this << color().YELLOW;
        } else {
            *this << color().CYAN;
        }
        *this << "=== Result ===" << fmt::LF << "passes: " << abs_tests() - faults() << '/' << abs_tests()
              << " failures: " << abs_fails() << '/' << abs_tests() << " errors: " << abs_errs() << '/' << abs_tests()
              << " (" << abs_time() << "ms)" << color() << fmt::LF;
    }
};
}  // namespace report
}  // namespace intern

using console_reporter = intern::report::console_reporter;
}  // namespace tpp

#endif  // TPP_REPORT_CONSOLE_REPORTER_HPP
