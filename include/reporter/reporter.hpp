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
#include <string>

#include "testsuite/testsuite.hpp"

namespace sctf
{
namespace intern
{
namespace fmt
{
static constexpr char const* const SPACE = " ";   ///< Single space
static constexpr char const* const LF    = "\n";  ///< Single linefeed
namespace ansi
{
static constexpr char const* const RED    = "\x1b[31m";
static constexpr char const* const GREEN  = "\x1b[32m";
static constexpr char const* const YELLOW = "\x1b[33m";
static constexpr char const* const BLUE   = "\x1b[34m";
static constexpr char const* const CYAN   = "\x1b[36m";
static constexpr char const* const RST    = "\x1b[0m";
}  // namespace ansi
}  // namespace fmt

class reporter;
using reporter_ptr = std::shared_ptr<reporter>;
/**
 * Abstract base class for specific reporter implementations.
 */
class reporter : public std::enable_shared_from_this<reporter>
{
public:
    reporter(reporter const&)     = delete;
    reporter(reporter&&) noexcept = delete;
    virtual ~reporter() noexcept  = default;
    auto
    operator=(reporter const&) -> reporter& = delete;
    auto
    operator=(reporter&&) noexcept -> reporter& = delete;

    /**
     * Generate the report part for a single testsuite.
     *
     * @param ts_ is the testsuite to report.
     */
    void
    report(testsuite_ptr const& ts_) {
        report_testsuite(ts_);
        m_out_stream.flush();
    }

    /**
     * Generate the report prologue.
     * This method may be overridden in derived classes, but must be called first in there.
     */
    virtual void
    begin_report() {
        m_abs_errs  = 0;
        m_abs_fails = 0;
        m_abs_tests = 0;
        m_abs_time  = 0.0;
    };

    /**
     * Generate the report epilogue.
     * This method must be implemented in derived classes.
     */
    virtual void
    end_report() = 0;

    /**
     * Get the number of failures and errors.
     *
     * @return the sum of failures and errors.
     */
    auto
    faults() const -> std::size_t {
        return m_abs_errs + m_abs_fails;
    }

    /**
     * Enable ansi colors in report.
     *
     * @return this reporter again.
     */
    auto
    with_color() -> reporter_ptr {
        m_color = true;
        return shared_from_this();
    }

    /**
     * Enable reporting of captured output.
     *
     * @return this reporter again.
     */
    auto
    with_captured_output() -> reporter_ptr {
        m_capture = true;
        return shared_from_this();
    }

    auto
    with_stripping() -> reporter_ptr {
        m_stripped = true;
        return shared_from_this();
    }

protected:
    /// Helper type to prevent public constructor usage.
    struct enable
    {};

    /**
     * @param stream_ is the output stream for reports.
     */
    explicit reporter(std::ostream& stream_) : m_out_stream(stream_) {
        if (!m_out_stream) {
            throw std::runtime_error("could not open stream for report");
        }
    }

    /**
     * @param fname_ is the output filename for reports.
     * @throw std::runtime_error if the file can not be opened for writing.
     */
    explicit reporter(std::string const& fname_) : m_out_file(fname_), m_out_stream(m_out_file) {
        if (!m_out_stream) {
            throw std::runtime_error("could not open file for report");
        }
    }

    /**
     * Generate a part of the report for a single testsuite.
     * This method is used for every testsuite. It may be overridden in derived classes.
     *
     * @param ts_ is the testsuite to generate the report for.
     */
    virtual void
    report_testsuite(testsuite_ptr const& ts_) {
        m_abs_errs += ts_->statistics().errors();
        m_abs_fails += ts_->statistics().failures();
        m_abs_tests += ts_->statistics().tests();
        m_abs_time += ts_->execution_duration();
        std::for_each(ts_->testcases().begin(), ts_->testcases().end(),
                      [this](testcase const& tc) { report_testcase(tc); });
    }

    /**
     * Generate a part of the report for a single testcase.
     * This method is used for every testcase. It must be implemented in derived classes.
     *
     * @param tc_ is the testcase to generate the report for.
     */
    virtual void
    report_testcase(testcase const& tc_) = 0;

    /**
     * Print anything to the specified output stream, or file.
     *
     * @tparam T is the type of t_.
     * @param t_ is the thing to print.
     */
    template<typename T>
    auto
    operator<<(T const& t_) -> std::ostream& {
        m_out_stream << t_;
        return m_out_stream;
    }

    auto
    space(std::uint32_t depth_ = 1) -> char const* {
        if (!m_stripped) {
            for (auto i = 0U; i < depth_; ++i) {
                *this << fmt::SPACE;
            }
        }
        return "";
    }

    auto
    newline(std::uint32_t depth_ = 1) -> char const* {
        if (!m_stripped) {
            for (auto i = 0U; i < depth_; ++i) {
                *this << fmt::LF;
            }
        }
        space(m_indent_lvl);
        return "";
    }

    void
    push_indent(std::uint32_t lvl_ = 2) {
        m_indent_lvl += lvl_;
    }

    void
    pop_indent(std::uint32_t lvl_ = 2) {
        m_indent_lvl -= lvl_;
    }

    auto
    capture() const -> bool {
        return m_capture;
    }

    auto
    abs_tests() const -> std::size_t {
        return m_abs_tests;
    }

    auto
    abs_fails() const -> std::size_t {
        return m_abs_fails;
    }

    auto
    abs_errs() const -> std::size_t {
        return m_abs_errs;
    }

    auto
    abs_time() const -> double {
        return m_abs_time;
    }

    enum colors
    {
        RED,
        GREEN,
        YELLOW,
        BLUE,
        CYAN,
        RESET
    };

    auto
    color(colors c_ = RESET) const -> char const* {
        if (!m_color) {
            return "";
        }
        switch (c_) {
            case RED: return fmt::ansi::RED;
            case GREEN: return fmt::ansi::GREEN;
            case YELLOW: return fmt::ansi::YELLOW;
            case BLUE: return fmt::ansi::BLUE;
            case CYAN: return fmt::ansi::CYAN;
            default: return fmt::ansi::RST;
        }
    }

private:
    std::ofstream m_out_file;    ///< Filestream that is used, if a file is specified as output target.
    std::ostream& m_out_stream;  ///< Outstream that handles printing the report.
    std::uint32_t m_indent_lvl = 0;
    bool          m_color      = false;  ///< Flags whether print colored results.
    bool          m_capture    = false;  ///< Flags whether to report captured output from testcases.
    bool          m_stripped   = false;  ///< Flags whether to strip unnecessary whitespaces in report.
    std::size_t   m_abs_tests  = 0;      ///< Total number of testcases over all testsuites.
    std::size_t   m_abs_fails  = 0;      ///< Total number of failed testcases over all testsuites.
    std::size_t   m_abs_errs   = 0;      ///< Total number of erroneous testcases over all testsuites.
    double        m_abs_time   = 0;      ///< Total amount of time spent on all testsuites.
};
}  // namespace intern

using reporter_ptr = intern::reporter_ptr;
}  // namespace sctf

#endif  // SCTF_REPORTER_REPORTER_HPP
