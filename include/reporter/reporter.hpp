/*
 Copyright_License {

 Copyright (C) 2017 Julian P. Becht
 Author: Julian P. Becht

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License version 3
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 }
 */

#ifndef SCTF_REPORTER_REPORTER_HPP
#define SCTF_REPORTER_REPORTER_HPP

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <ostream>
#include <stdexcept>
#include <vector>

#include "testsuite/statistic.hpp"
#include "testsuite/testsuite.hpp"

#include "runner.hpp"
#include "types.hpp"

#define SCTF_LF "\n"
#define SCTF_SPACE "  "
#define SCTF_XSPACE "    "

namespace sctf
{
namespace private_
{
/**
 * Abstract reporter type. Used as base class for specific reporter implementations.
 */
class reporter
{
public:
    virtual ~reporter() noexcept = default;

    /**
     * Generate the report.
     * As side effect, all pending testcases in runner_ are executed.
     * @return the sum of failed tests and errors
     */
    std::size_t report() {
        m_abs_errs     = 0;
        m_abs_fails    = 0;
        m_abs_tests    = 0;
        m_abs_time     = 0.0;
        runner& runner = runner::instance();

        runner.run();
        begin_report();
        std::for_each(runner.testsuites().begin(), runner.testsuites().end(),
                      [this](testsuite_ptr const& ts_) {
                          m_abs_errs += ts_->statistics().errors();
                          m_abs_fails += ts_->statistics().failures();
                          m_abs_tests += ts_->statistics().tests();
                          m_abs_time += ts_->execution_time();
                          report_testsuite(ts_);
                      });
        end_report();
        return m_abs_errs + m_abs_fails;
    }

protected:
    explicit reporter(std::ostream& stream_) : mr_out_stream(stream_) {}
    explicit reporter(char const* fname_) : m_out_file(fname_), mr_out_stream(m_out_file) {
        if (!mr_out_stream) {
            throw std::runtime_error("Could not open file.");
        }
    }

    inline virtual void report_testsuite(testsuite_ptr const ts_) {
        std::for_each(ts_->testcases().begin(), ts_->testcases().end(),
                      [this](const testcase& tc) { report_testcase(tc); });
    }

    virtual void report_testcase(testcase const& tc_) = 0;
    virtual void begin_report()                       = 0;
    virtual void end_report()                         = 0;

    template<typename T>
    std::ostream& operator<<(T const& t_) {
        mr_out_stream << t_;
        return mr_out_stream;
    }

    std::ofstream m_out_file;
    std::ostream& mr_out_stream;
    std::size_t   m_abs_tests = 0;
    std::size_t   m_abs_fails = 0;
    std::size_t   m_abs_errs  = 0;
    double        m_abs_time  = 0;
};
}  // namespace private_
}  // namespace sctf

#endif  // SCTF_REPORTER_REPORTER_HPP
