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
#include "../util/timing.hpp"
#include "../util/types.h"

namespace testsuite
{

/**
 * Data-class representing a TestCase.
 * Non-copyable
 */
class TestCase
{
public:
    /**
     * c'tor
     */
    inline TestCase(const std::string& name, const std::string& classn,
                    test_function f)
            : name(name),
              classname(classn),
              func(f)
    {
    }

    /**
     * d'tor
     */
    inline virtual ~TestCase() throw ()
    {
    }

    enum States
        : std::uint32_t
        {
            NONE,
        PASSED,
        FAILED,
        ERROR
    };

    inline States execute() noexcept
    {
        util::timing::duration dur;
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
     * Test results
     */
    States state = NONE;

    /**
     * Test runtime in
     * nanoseconds
     */
    std::uint64_t duration = 0;

    /**
     * name/description
     */
    const std::string name;

    /**
     * classname
     * if tested method is member -> instance classname
     * else testsuite name
     */
    const std::string classname;

    /**
     * Test value as string, holds
     * - error msg, in case of error
     */
    std::string errmsg;

    test_function func;

private:
    /**
     * Test passed or failed
     */
    inline void pass()
    {
        state = PASSED;
    }

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

    inline void erroneous()
    {
        erroneous("");
    }
};

} // testsuite

#endif /* TESTSUITE_TESTCASE_HPP_ */
