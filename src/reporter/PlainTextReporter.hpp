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
        std::uint64_t abs_time = 0;
        for (auto ts : suites)
        {
            abs_tests += ts->stats.num_of_tests;
            abs_fails += ts->stats.num_of_fails;
            abs_time += ts->time;
            *this << "Run Testsuite [" << ts->name << "]; time = " << (double)ts->time/1000.0 << "ms" << LF;
            for (auto tc : ts->testcases)
            {
                *this << SPACE << "Run Testcase [" << tc.name << "](" << tc.value
                      << ") with ( ";
                for (auto arg = tc.args.begin(); arg != tc.args.end(); arg++)
                {
                    *this << *arg;
                    if (arg < tc.args.end()-1)
                    {
                        *this << " , ";
                    }
                }
                *this << " ); time = " << (double)tc.time/1000.0 << "ms" << LF;
                *this << SPACE << SPACE << "[" << tc.name << "] ";
                if (!tc.passed)
                {
                    *this << "failed!; expected = \"" << tc.expected << "\"" << LF;
                }
                else
                {
                    *this << "passed!" << LF;
                }
            }
        }
        *this << "Result:: passed: " << abs_tests - abs_fails << "/" << abs_tests
              << " ; failed: " << abs_fails << "/" << abs_tests << " ; time = " << (double)abs_time/1000.0 << "ms" << LF;
    }
};

#endif /* REPORTER_PLAINTEXTREPORTER_HPP_ */
