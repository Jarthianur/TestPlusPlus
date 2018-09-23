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

#ifndef SCTF_SRC_REPORTER_ABSTRACTREPORTER_HPP_
#define SCTF_SRC_REPORTER_ABSTRACTREPORTER_HPP_

#include <cstddef>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "../testsuite/TestStats.hpp"
#include "../testsuite/TestSuite.hpp"
#include "../testsuite/TestSuitesRunner.hpp"
#include "../types.h"

namespace sctf
{
namespace rep
{
/**
 * @def LF
 * @brief Line feed
 */
#ifdef _WIN32
#define LF "\r\n"
#else
#define LF "\n"
#endif

/**
 * @def SPACE
 * @brief Spacing with two spaces
 */
#define SPACE "  "

/**
 * @def XSPACE
 * @brief Spacing with four spaces
 */
#define XSPACE "    "

/**
 * @brief Report testsuites in a format specified by concrete reporter types.
 */
class AbstractReporter
{
public:
    /**
     * @brief Generate report.
     * @param runner The TestSuitesRunner to generate reports for
     * @note Executes runner's pending TestSuite.
     * @return the sum of failed tests and errors
     */
    std::size_t report(test::TestSuitesRunner& runner)
    {
        m_abs_errs  = 0;
        m_abs_fails = 0;
        m_abs_tests = 0;
        m_abs_time  = 0.0;

        runner.run();
        begin_report();
        const std::vector<TestSuite_shared>& testsuites = runner.testsuites();
        for(const TestSuite_shared& ts : testsuites)
        {
            m_abs_errs += ts->statistics().errors();
            m_abs_fails += ts->statistics().failures();
            m_abs_tests += ts->statistics().tests();
            m_abs_time += ts->time();
            report_ts(ts);
        }
        end_report();
        return m_abs_errs + m_abs_fails;
    }

protected:
    /**
     * @var m_out_file
     * @brief The output file stream
     */
    std::ofstream m_out_file;

    /**
     * @var mr_out_stream
     * @brief The output stream reference
     */
    std::ostream& mr_out_stream;

    /**
     * @brief Constructor
     * @param stream The out-stream to report to
     */
    explicit AbstractReporter(std::ostream& stream) : mr_out_stream(stream)
    {}

    /**
     * @brief Constructor
     * @param fname The filename where to report to
     * @throw std::runtime_error if the file cannot be opened for writing
     */
    explicit AbstractReporter(const char* fname)
        : m_out_file(fname), mr_out_stream(m_out_file)
    {
        if(!mr_out_stream)
        {
            throw std::runtime_error("Could not open file.");
        }
    }

    /**
     * @brief Destructor
     */
    virtual ~AbstractReporter() noexcept
    {}

    /**
     * @brief Generate report for a given TestSuite.
     * @param ts The TestSuite
     */
    inline virtual void report_ts(const TestSuite_shared ts)
    {
        for(auto& tc : ts->testcases())
        {
            report_tc(tc);
        }
    }

    /**
     * @brief Generate report for a given TestCase.
     * @param tc The TestCase
     */
    virtual void report_tc(const test::TestCase& tc) = 0;

    /**
     * @brief Generate the intro of a report.
     */
    virtual void begin_report() = 0;

    /**
     * @brief Generate the outro of a report.
     */
    virtual void end_report() = 0;

    /**
     * @brief Write to stream.
     * @tparam Type of what to write to stream
     * @param _1 The element to write
     */
    template<typename T>
    std::ostream& operator<<(const T& _1)
    {
        mr_out_stream << _1;
        return mr_out_stream;
    }

    inline std::size_t abs_tests() const
    {
        return m_abs_tests;
    }

    inline std::size_t abs_fails() const
    {
        return m_abs_fails;
    }

    inline std::size_t abs_errs() const
    {
        return m_abs_errs;
    }

    inline double abs_time() const
    {
        return m_abs_time;
    }

private:
    /// @brief The amount of tests.
    std::size_t m_abs_tests = 0;

    /// @brief The amount of failed tests.
    std::size_t m_abs_fails = 0;

    /// @brief The amount of erroneous tests.
    std::size_t m_abs_errs = 0;

    /// @brief The accumulated runtime.
    double m_abs_time = 0;
};

}  // namespace rep
}  // namespace sctf

#endif  // SCTF_SRC_REPORTER_ABSTRACTREPORTER_HPP_
