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
#include <string>

#include "../util/AssertionFailure.hpp"
#include "../util/duration.hpp"
#include "../util/types.h"

namespace testsuite
{

/**
 * Data-class representing a TestCase.
 */
class TestCase
{
public:
    /**
     * c'tor
     * name: name/descr of test case
     * classn: classname/context of test function
     * f: test function, exec ops and asserts
     */
    inline TestCase(const std::string& name, const std::string& classn, test_function f)
            : name(name),
              classname(classn),
              func(f)
    {
    }

    /**
     * d'tor
     */
    inline virtual ~TestCase() noexcept
    {
    }

    /**
     * Test case state
     */
    enum States
        : std::uint32_t
        {
            /**
             * not yet executed
             */
            NONE,
        /**
         * executed and passed
         */
        PASSED,
        /**
         * executed and failed
         */
        FAILED,
        /**
         * executed with error
         */
        ERROR
    };

    /**
     * Execute test function, store results.
     */
    inline States execute() noexcept
    {
        util::duration dur;
        try
        {
            func();
            pass();
        }
        catch (const AssertionFailure& e)
        {
            fail(e.what());
        }
        catch (const std::exception& e)
        {
            erroneous(e.what());
        }
        catch (...)
        {
            erroneous();
        }
        duration = dur.get();
        return state;
    }

    /**
     * Test state
     */
    States state = NONE;

    /**
     * Test duration in
     * milliseconds
     */
    double duration = 0.0;

    /**
     * name/description
     */
    const std::string name;

    /**
     * classname
     * if tested member functions -> instance classname
     * else context name
     */
    const std::string classname;

    /**
     * error msg, in case of error
     */
    std::string errmsg;

private:
    /**
     * Test passed
     */
    inline void pass()
    {
        state = PASSED;
    }

    /**
     * Failed due to what.
     * msg: reason
     */
    inline void fail(const char* msg)
    {
        state = FAILED;
        errmsg = msg;
    }

    /**
     * Any error occurred while test run.
     * err: error msg
     */
    inline void erroneous(const std::string& err)
    {
        state = ERROR;
        errmsg = err;
    }

    /**
     * Same but without specified err msg.
     */
    inline void erroneous()
    {
        erroneous("");
    }

    /**
     * test function
     */
    test_function func;
};

} // testsuite

#endif /* TESTSUITE_TESTCASE_HPP_ */
