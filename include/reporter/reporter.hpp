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

/// @file

#ifndef SCTF_REPORTER_REPORTER_HPP
#define SCTF_REPORTER_REPORTER_HPP

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <memory>
#include <ostream>
#include <stdexcept>

#include "testsuite/testsuite.hpp"

#include "runner.hpp"

namespace sctf
{
namespace intern
{
/**
 * Abstract base class for specific reporter implementations.
 */
class reporter
{
public:
    reporter(reporter const&)     = delete;
    reporter(reporter&&) noexcept = delete;
    reporter& operator=(reporter const&) = delete;
    reporter& operator=(reporter&&) noexcept = delete;
    virtual ~reporter() noexcept             = default;

    /**
     * Generate the report. Testsuites that are not yet completed will be run by a call to this
     * function.
     *
     * @return the sum of failed and erroneous tests
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
                          m_abs_time += ts_->execution_duration();
                          report_testsuite(ts_);
                      });
        end_report();
        return m_abs_errs + m_abs_fails;
    }

protected:
    /**
     * @param stream_ is the output stream for reports.
     */
    explicit reporter(std::ostream& stream_) : m_out_stream(stream_) {}

    /**
     * @param fname_ is the output filename for reports.
     * @throw std::runtime_error if the file can not be opened for writing.
     */
    explicit reporter(char const* fname_) : m_out_file(fname_), m_out_stream(m_out_file) {
        if (!m_out_stream) {
            throw std::runtime_error("Could not open file.");
        }
    }

    /**
     * Generate a part of the report for a single testsuite.
     * This method is used for every testsuite. It may be overridden in derived classes.
     *
     * @param ts_ is the testsuite to generate the report for.
     */
    virtual void report_testsuite(testsuite_ptr const ts_) {
        std::for_each(ts_->testcases().begin(), ts_->testcases().end(),
                      [this](const testcase& tc) { report_testcase(tc); });
    }

    /**
     * Generate a part of the report for a single testcase.
     * This method is used for every testcase. It must be implemented in derived classes.
     *
     * @param tc_ is the testcase to generate the report for.
     */
    virtual void report_testcase(testcase const& tc_) = 0;

    /**
     * Generate the report prologue. This method must be implemented in derived classes.
     */
    virtual void begin_report() = 0;

    /**
     * Generate the report epilogue. This method must be implemented in derived classes.
     */
    virtual void end_report() = 0;

    /**
     * Print anything to the specified output stream, or file.
     *
     * @tparam T is the type of t_.
     * @param t_ is the thing to print.
     */
    template<typename T>
    std::ostream& operator<<(T const& t_) {
        m_out_stream << t_;
        return m_out_stream;
    }

    std::ofstream
                  m_out_file;  ///< Filestream that is used, if a file is specified as output target.
    std::ostream& m_out_stream;     ///< Outstream that handles printing the report.
    std::size_t   m_abs_tests = 0;  ///< Total number of testcases over all testsuites.
    std::size_t   m_abs_fails = 0;  ///< Total number of failed testcases over all testsuites.
    std::size_t   m_abs_errs  = 0;  ///< Total number of erroneous testcases over all testsuites.
    double        m_abs_time  = 0;  ///< Total amount of time spent on all testsuites.
};

namespace fmt
{
static constexpr char const* const SPACE  = "  ";    ///< Indentation with two spaces.
static constexpr char const* const XSPACE = "    ";  ///< Indentation with four spaces.
static constexpr char const* const LF     = "\n";    ///< Single linefeed.
static constexpr char const* const XLF    = "\n\n";  ///< Double linefeed.
}  // namespace fmt
}  // namespace intern

using reporter_ptr = std::shared_ptr<intern::reporter>;
}  // namespace sctf

#endif  // SCTF_REPORTER_REPORTER_HPP
