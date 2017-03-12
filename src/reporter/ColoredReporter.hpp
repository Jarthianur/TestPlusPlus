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

#ifndef REPORTER_COLOREDREPORTER_HPP_
#define REPORTER_COLOREDREPORTER_HPP_

#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "../testsuite/TestCase.hpp"
#include "../testsuite/TestStats.hpp"
#include "../testsuite/TestSuite.hpp"
#include "AbstractReporter.hpp"

namespace testsuite
{
namespace reporter
{

// ANSI colors
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
// ANSI reset colors
#define ANSI_RESET   "\x1b[0m"

/**
 * Concrete reporter,
 * featuring colored plain text format.
 */
class ColoredReporter: public AbstractReporter
{
public:
    /**
     * c'tor with stream
     */
    inline ColoredReporter(std::ostream& stream)
            : AbstractReporter(stream)
    {
    }

    /**
     * d'tor
     */
    inline virtual ~ColoredReporter() throw ()
    {
    }

    /**
     * Implementation of generate()
     */
    inline virtual std::int32_t generate()
    {
        std::uint32_t abs_tests = 0;
        std::uint32_t abs_fails = 0;
        std::uint64_t abs_time = 0;
        for (auto ts : suites)
        {
            abs_tests += ts->stats.num_of_tests;
            abs_fails += ts->stats.num_of_fails;
            abs_time += ts->time;
            *this << "Run Testsuite [" << ts->name << "]; time = "
                  << (double) ts->time / 1000.0 << "ms" << LF;
            for (auto tc : ts->testcases)
            {
                *this << SPACE << "Run Testcase [" << tc->name << "](" << tc->value
                      << ") with ( ";
                for (auto arg = tc->args.begin(); arg != tc->args.end(); arg++)
                {
                    *this << *arg;
                    if (arg < tc->args.end() - 1)
                    {
                        *this << " , ";
                    }
                }
                *this << " ); time = " << (double) tc->time / 1000.0 << "ms" << LF;
                if (!tc->passed)
                {
                    *this << SPACE << SPACE << ANSI_RED << "[" << tc->name << "] "
                          << "failed!; expected = \"" << tc->expected << "\""
                          << ANSI_RESET << LF;
                }
                else
                {
                    *this << SPACE << SPACE << ANSI_GREEN << "[" << tc->name << "] "
                          << "passed!" << ANSI_RESET << LF;
                }
            }
        }
        if (abs_fails >= (abs_tests + 1) / 2)
        {
            *this << ANSI_YELLOW;
        }
        else
        {
            *this << ANSI_CYAN;
        }
        *this << "Result:: passed: " << abs_tests - abs_fails << "/" << abs_tests
              << " ; failed: " << abs_fails << "/" << abs_tests << " ; time = "
              << (double) abs_time / 1000.0 << "ms" << ANSI_RESET << LF;
        return abs_fails;
    }

};

} // reporter
} // testsuite

#endif /* REPORTER_COLOREDREPORTER_HPP_ */
