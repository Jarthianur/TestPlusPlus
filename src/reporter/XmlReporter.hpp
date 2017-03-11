#ifndef REPORTER_XMLREPORTER_HPP_
#define REPORTER_XMLREPORTER_HPP_

#include <chrono>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "../testsuite/TestCase.hpp"
#include "../testsuite/TestStats.hpp"
#include "../testsuite/TestSuite.hpp"
#include "AbstractReporter.hpp"

#ifdef __GNUC__
#define GNU_MA __GNUC__
#define GNU_MI __GNUC_MINOR__
#elif
#define GNU_MA "X"
#define GNU_MI "X"
#endif

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

    inline virtual void generate()
    {
        *this << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << LF << "<testsuites>"
              << LF;

        std::uint32_t abs_tests = 0;
        std::uint32_t abs_fails = 0;
        std::uint64_t abs_time = 0;
        for (auto ts : suites)
        {
            abs_tests += ts->stats.num_of_tests;
            abs_fails += ts->stats.num_of_fails;
            abs_time += ts->time;
            std::time_t stamp = std::chrono::high_resolution_clock::to_time_t(
                    ts->timestamp);
            char buff[128];
            std::strftime(buff, 127, "%FT%T", std::localtime(&stamp));

            *this << SPACE << "<testsuite name=\"" << ts->name
                  << "\" errors=\"0\" tests=\"" << ts->stats.num_of_tests
                  << "\" failures=\"" << "\" time=\"" << (double) ts->time / 1000
                  << "\" timestamp=\"" << buff << "\" />" << LF << XSPACE
                  << "<properties>" << LF << XSPACE << SPACE
                  << "<property name=\"Compiler\" value=\"GNU G++-" << GNU_MA << "."
                  << GNU_MI << "\"/>" << LF << XSPACE << "</properties>" << LF;

            for (auto tc : ts->testcases)
            {
                *this << SPACE << "Run Testcase [" << tc.name << "](" << tc.value
                      << ") with ( ";

                for (auto arg = tc.args.begin(); arg != tc.args.end(); arg++)
                {
                    *this << *arg;

                    if (arg < tc.args.end() - 1)
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
              << " ; failed: " << abs_fails << "/" << abs_tests << " ; time = "
              << abs_time << "ms" << LF;
    }
};

#endif /* REPORTER_XMLREPORTER_HPP_ */
