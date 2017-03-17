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

#ifndef REPORTER_ABSTRACTREPORTER_HPP_
#define REPORTER_ABSTRACTREPORTER_HPP_

#include <cstdint>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "../testsuite/TestStats.hpp"
#include "../testsuite/TestSuite.hpp"
#include "../testsuite/TestSuitesRunner.hpp"
#include "../util/types.h"

namespace testsuite
{
namespace reporter
{

#define LF "\n"
#define SPACE "  "
#define XSPACE "    "

/**
 * Abstract reporter type.
 * Reports tests to given out-stream,
 * in format specified by concrete types generate method.
 */
class AbstractReporter
{

public:
    /**
     * c'tor with target out-stream.
     */
    inline AbstractReporter(std::ostream& stream)
            : out_file(),
              out_stream(stream)
    {
    }

    /**
     * c'tor with target filename
     */
    inline AbstractReporter(const char* fnam)
            : out_file(fnam),
              out_stream(this->out_file)
    {
        if (!out_stream)
        {
            throw std::runtime_error("Could not open file.");
        }
    }

    /**
     * d'tor
     */
    inline virtual ~AbstractReporter() noexcept
    {
    }

    /**
     * Generate report and return sum
     * of failed tests and errors.
     * Executes runners test suites, if not done yet.
     */
    inline std::int32_t report(TestSuitesRunner& runner)
    {
        if (runner.getStatus() == TestSuitesRunner::ALL)
        {
            std::int32_t ret_val = 0;
            beginReport();

            auto ts_pair = runner.getTestSuites();

            for (auto ts : ts_pair.first)
            {
                reportTestSuite(ts);
                ret_val += ts->getTestStats().getNumFails()
                        + ts->getTestStats().getNumErrs();
            }
            for (auto ts : ts_pair.second)
            {
                reportTestSuite(ts);
                ret_val += ts->getTestStats().getNumFails()
                        + ts->getTestStats().getNumErrs();
            }

            endReport();
            return ret_val;
        }
        else
        {
            runner.executeAll();
            return report(runner);
        }
    }

protected:
    /**
     * Target out-stream
     */
    std::ofstream out_file;
    std::ostream& out_stream;

    /**
     * Generate report format for given test suite.
     * This parent method may be overridden and called from
     * derived class to generate report for each contained test case.
     */
    inline virtual void reportTestSuite(TestSuite_shared ts)
    {
        for (auto& tc : ts->getTestCases())
        {
            reportTestCase(tc);
        }
    }

    /**
     * Generate report format for given test case.
     */
    virtual void reportTestCase(const TestCase& tc) = 0;

    /**
     * Generate intro report format.
     */
    virtual void beginReport() = 0;

    /**
     * Generate outro report format.
     */
    virtual void endReport() = 0;

    /**
     * Write to stream.
     * Chainable
     */
    template<typename T>
    inline std::ostream& operator<<(const T& rep)
    {
        out_stream << rep;
        return out_stream;
    }
};

} // reporter
} // testsuite

#endif /* REPORTER_ABSTRACTREPORTER_HPP_ */
