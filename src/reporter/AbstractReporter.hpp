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
#include <iostream>
#include <memory>
#include <vector>

#include "../testsuite/TestSuite_shared.h"

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
            : out_stream(stream)
    {
    }

    /**
     * d'tor
     */
    inline virtual ~AbstractReporter() throw ()
    {
    }

    /**
     * Generate report and return sum
     * of failed tests and errors.
     */
    inline std::int32_t report()
    {
        return generate();
    }

    /**
     * Register a testsuite.
     */
    inline void registerTestSuite(TestSuite_shared ts)
    {
        suites.push_back(ts);
    }

protected:
    /**
     * Target out-stream
     */
    std::ostream& out_stream;

    /**
     * Registered testsuites
     */
    std::vector<TestSuite_shared> suites;

    /**
     * Generate report in concrete type.
     * Return sum of failed and erroneous tests.
     */
    virtual std::int32_t generate() = 0;

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

/**
 * Typedef for shared ptr
 */
using AbstractReporter_shared = std::shared_ptr<AbstractReporter>;

} // reporter
} // testsuite

#endif /* REPORTER_ABSTRACTREPORTER_HPP_ */
