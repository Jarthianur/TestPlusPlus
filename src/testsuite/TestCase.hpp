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
    TestCase(const std::string& name, const std::string& classn, test_function f)
        : mName(name), mClassname("test." + classn), mTestFunc(f)
    {}

    /**
     * d'tor
     */
    virtual ~TestCase() noexcept
    {}

    /**
     * Test case state
     */
    enum States : std::uint32_t
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
    States execute() noexcept
    {
        util::duration dur;
        try
        {
            mTestFunc();
            pass();
        }
        catch(const AssertionFailure& e)
        {
            fail(e.what());
        }
        catch(const std::exception& e)
        {
            erroneous(e.what());
        }
        catch(...)
        {
            erroneous();
        }
        mDuration = dur.get();
        return mState;
    }

    inline const States getState() const
    {
        return mState;
    }

    inline const double getDuration() const
    {
        return mDuration;
    }

    inline const std::string& getErrMsg() const
    {
        return mErrMsg;
    }

    const std::string mName;
    const std::string mClassname;

private:
    /**
     * Test passed
     */
    inline void pass()
    {
        mState = PASSED;
    }

    /**
     * Failed due to what.
     * msg: reason
     */
    inline void fail(const char* msg)
    {
        mState  = FAILED;
        mErrMsg = msg;
    }

    /**
     * Any error occurred while test run.
     * err: error msg
     */
    inline void erroneous(const std::string& err = "")
    {
        mState  = ERROR;
        mErrMsg = err;
    }

    States mState = NONE;
    /**
     * milliseconds
     */
    double mDuration = 0.0;
    std::string mErrMsg;
    test_function mTestFunc;
};

}  // testsuite

#endif /* TESTSUITE_TESTCASE_HPP_ */
