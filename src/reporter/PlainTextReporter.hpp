/*
 * PlainTextReporter.hpp
 *
 *  Created on: 10.03.2017
 *      Author: julian
 */

#ifndef REPORTER_PLAINTEXTREPORTER_HPP_
#define REPORTER_PLAINTEXTREPORTER_HPP_

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include "../testsuite/TestCase.hpp"
#include "../testsuite/TestStats.hpp"
#include "../testsuite/TestSuite.hpp"
#include "AbstractReporter.hpp"

class PlainTextReporter: public AbstractReporter
{
public:
    inline PlainTextReporter(std::ostream& stream)
            : AbstractReporter(stream)
    {
    }

    inline virtual ~PlainTextReporter() throw ()
    {
    }

    inline virtual void generate()
    {
        std::uint32_t abs_tests = 0;
        std::uint32_t abs_fails = 0;
        for (auto ts : suites)
        {
            abs_tests += ts->stats.num_of_tests;
            abs_fails += ts->stats.num_of_fails;
            *this << "Run Testsuite [" << ts->name << "]" << LF;
            for (auto tc : ts->testcases)
            {
                *this << SPACE << "Run Testcase [" << tc.name << "] : "
                      << (tc.failed ? "failed" : "passed") << LF;
            }
        }
        *this << "Result:: passed: " << abs_tests - abs_fails << "/" << abs_tests
              << " ; failed: " << abs_fails << "/" << abs_tests << LF;
    }
};

#endif /* REPORTER_PLAINTEXTREPORTER_HPP_ */
