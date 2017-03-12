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

#ifndef TESTSUITE_TESTCASE_HPP_
#define TESTSUITE_TESTCASE_HPP_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace testsuite
{

class TestCase;
/**
 * Typedef for shared ptr to TestCase.
 */
using TestCase_shared = std::shared_ptr<TestCase>;

/**
 * Data-class representing a TestCase.
 * Non-copyable
 */
class TestCase
{
public:
    TestCase(const TestCase&) = delete;
    TestCase& operator=(const TestCase&) = delete;

    /**
     * Factory method for TestCase.
     * name: representative name
     * expected: expected value as string
     * args: arguments given to a method to test as strings
     * assert: comparators assertion
     */
    inline static TestCase_shared create(const std::string& name,
                                         const std::string& expected,
                                         const std::vector<std::string>& args,
                                         const std::string& assert)
    {
        return TestCase_shared(new TestCase(name, expected, args, assert));
    }

    /**
     * d'tor
     */
    inline virtual ~TestCase() throw ()
    {
    }

    /**
     * Test passed or failed
     */
    inline void pass(bool pass)
    {
        passed = pass;
    }

    /**
     * Any error occurred while test run.
     * err: error msg
     */
    inline void erroneous(const std::string& err)
    {
        passed = false;
        error = true;
        value = err;
    }

    /**
     * Test results
     */
    bool passed = true;
    bool error = false;

    /**
     * Test runtime in
     * nanoseconds
     */
    std::uint64_t time = 0;

    /**
     * name/description
     */
    std::string name;

    /**
     * Test value as string, holds
     * - error msg, in case of error
     * - return value from method to test
     */
    std::string value;

    /**
     * Expected value as string
     */
    std::string expected;

    /**
     * Arguments given to method to test, as string
     */
    std::vector<std::string> args;

    /**
     * Assertion representation
     */
    const std::string assertion;

private:
    /**
     * c'tor
     */
    inline TestCase(const std::string& name, const std::string& expected,
                    const std::vector<std::string>& args, const std::string& assert)
            : name(name),
              expected(expected),
              args(args),
              assertion(assert)
    {
    }
};

} // testsuite

#endif /* TESTSUITE_TESTCASE_HPP_ */
