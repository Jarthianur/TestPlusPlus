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

#ifndef TPP_REPORT_REPORTER_HPP
#define TPP_REPORT_REPORTER_HPP

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>

#include "test/testsuite.hpp"

namespace tpp
{
namespace intern
{
namespace report
{
namespace fmt
{
static constexpr auto SPACE = " ";   ///< Single space
static constexpr auto LF    = "\n";  ///< Single linefeed
namespace ansi
{
static constexpr auto RED        = "\x1b[0;91m";
static constexpr auto GREEN      = "\x1b[0;92m";
static constexpr auto YELLOW     = "\x1b[0;93m";
static constexpr auto BLUE       = "\x1b[0;94m";
static constexpr auto CYAN       = "\x1b[0;96m";
static constexpr auto MAGENTA    = "\x1b[0;95m";
static constexpr auto WHITE_BOLD = "\x1b[1;97m";
static constexpr auto RST        = "\x1b[0m";
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
    report(test::testsuite_ptr const& ts_) {
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
        m_abs_time  = .0;
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
    inline auto
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
        m_colors.RED     = fmt::ansi::RED;
        m_colors.GREEN   = fmt::ansi::GREEN;
        m_colors.YELLOW  = fmt::ansi::YELLOW;
        m_colors.BLUE    = fmt::ansi::BLUE;
        m_colors.CYAN    = fmt::ansi::CYAN;
        m_colors.MAGENTA = fmt::ansi::MAGENTA;
        m_colors.W_BOLD  = fmt::ansi::WHITE_BOLD;
        m_colors.RESET   = fmt::ansi::RST;
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

    struct color_palette
    {
        char const* RED{""};
        char const* GREEN{""};
        char const* YELLOW{""};
        char const* BLUE{""};
        char const* CYAN{""};
        char const* MAGENTA{""};
        char const* W_BOLD{""};
        char const* RESET{""};

        operator char const *() const {
            return RESET;
        }
    };

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
    report_testsuite(test::testsuite_ptr const& ts_) {
        m_abs_errs += ts_->statistics().errors();
        m_abs_fails += ts_->statistics().failures();
        m_abs_tests += ts_->statistics().tests();
        m_abs_time += ts_->statistics().elapsed_time();
        std::for_each(ts_->testcases().begin(), ts_->testcases().end(),
                      [this](test::testcase const& tc_) { report_testcase(tc_); });
    }

    /**
     * Generate a part of the report for a single testcase.
     * This method is used for every testcase. It must be implemented in derived classes.
     *
     * @param tc_ is the testcase to generate the report for.
     */
    virtual void
    report_testcase(test::testcase const& tc_) = 0;

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

    inline auto
    space(std::uint32_t depth_ = 1) -> char const* {
        if (!m_stripped) {
            for (auto i{0U}; i < depth_; ++i) {
                *this << fmt::SPACE;
            }
        }
        return "";
    }

    inline auto
    newline(std::uint32_t depth_ = 1) -> char const* {
        if (!m_stripped) {
            for (auto i{0U}; i < depth_; ++i) {
                *this << fmt::LF;
            }
        }
        space(m_indent_lvl);
        return "";
    }

    inline void
    push_indent(std::uint32_t lvl_ = 2) {
        m_indent_lvl += lvl_;
    }

    inline void
    pop_indent(std::uint32_t lvl_ = 2) {
        m_indent_lvl -= lvl_;
    }

    inline auto
    capture() const -> bool {
        return m_capture;
    }

    inline auto
    abs_tests() const -> std::size_t {
        return m_abs_tests;
    }

    inline auto
    abs_fails() const -> std::size_t {
        return m_abs_fails;
    }

    inline auto
    abs_errs() const -> std::size_t {
        return m_abs_errs;
    }

    inline auto
    abs_time() const -> double {
        return m_abs_time;
    }

    auto
    color() const -> color_palette const& {
        return m_colors;
    }

private:
    std::ofstream m_out_file;    ///< Filestream that is used, if a file is specified as output target.
    std::ostream& m_out_stream;  ///< Outstream that handles printing the report.
    std::uint32_t m_indent_lvl{0};
    color_palette m_colors{};         ///< Flags whether print colored results.
    bool          m_capture{false};   ///< Flags whether to report captured output from testcases.
    bool          m_stripped{false};  ///< Flags whether to strip unnecessary whitespaces in report.
    std::size_t   m_abs_tests{0};     ///< Total number of testcases over all testsuites.
    std::size_t   m_abs_fails{0};     ///< Total number of failed testcases over all testsuites.
    std::size_t   m_abs_errs{0};      ///< Total number of erroneous testcases over all testsuites.
    double        m_abs_time{0};      ///< Total amount of time spent on all testsuites.
};
}  // namespace report
}  // namespace intern

using reporter_ptr = intern::report::reporter_ptr;
}  // namespace tpp

#endif  // TPP_REPORT_REPORTER_HPP
