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

        for (auto ts : suites)
        {
            std::time_t stamp = std::chrono::high_resolution_clock::to_time_t(
                    ts->timestamp);
            char buff[128];
            std::strftime(buff, 127, "%FT%T", std::localtime(&stamp));

            *this << SPACE << "<testsuite name=\"" << ts->name << "\" errors=\""
                    << ts->stats.num_of_errs << "\" tests=\"" << ts->stats.num_of_tests
                    << "\" failures=\"" << ts->stats.num_of_fails << "\" time=\""
                    << (double) ts->time / 1000 << "\" timestamp=\"" << buff << "\">"
                    << LF;

            for (auto tc : ts->testcases)
            {
                *this << XSPACE << "<testcase name=\"" << tc.name << "\" classname=\""
                        << ts->name << "\" time=\"" << (double) tc.time / 1000 << "\">"
                        << LF;

                if (tc.error)
                {
                    *this << XSPACE << XSPACE << "<error message=\"" << tc.value
                            << "\"></error>" << LF;
                }
                else if (!tc.passed)
                {
                    *this << XSPACE << XSPACE
                            << "<failure message=\"Assertion failed, expected ("
                            << tc.value << ") " << tc.assertion << " (" << tc.expected
                            << ")\"></failure>" << LF;
                }

                *this << XSPACE << "</testcase>" << LF;
            }
            *this << SPACE << "</testsuite>" << LF;
        }
        *this << "</testsuites>" << LF;
    }
};

#endif /* REPORTER_XMLREPORTER_HPP_ */
