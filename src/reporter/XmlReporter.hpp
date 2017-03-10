#ifndef REPORTER_XMLREPORTER_HPP_
#define REPORTER_XMLREPORTER_HPP_


#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include "../testsuite/TestCase.hpp"
#include "../testsuite/TestStats.hpp"
#include "../testsuite/TestSuite.hpp"
#include "AbstractReporter.hpp"

class XmlReporter: public AbstractReporter
{
public:
    inline XmlReporter(std::ostream& stream)
            : AbstractReporter(stream)
    {
    }

    inline virtual ~XmlReporter() throw ()
    {
    }

    inline virtual void generate()// TODO
    {
        /*std::uint32_t abs_tests = 0;
        std::uint32_t abs_fails = 0;
        std::uint64_t abs_time = 0;
        for (auto ts : suites)
        {
            abs_tests += ts->stats.num_of_tests;
            abs_fails += ts->stats.num_of_fails;
            abs_time += ts->time;
            *this << "Run Testsuite [" << ts->name << "]; time = " << ts->time << "ms" << LF;
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
                *this << " ); time = " << tc.time << "ms" << LF;
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
              << " ; failed: " << abs_fails << "/" << abs_tests << " ; time = " << abs_time << "ms" << LF;*/
    }
};

#endif /* REPORTER_XMLREPORTER_HPP_ */
