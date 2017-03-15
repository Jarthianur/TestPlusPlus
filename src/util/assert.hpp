/*
 * assert.hpp
 *
 *  Created on: 15.03.2017
 *      Author: julian
 */

#ifndef SRC_UTIL_ASSERT_HPP_
#define SRC_UTIL_ASSERT_HPP_

#include <string>
#include <typeinfo>

#include "../comparator/ComparatorStrategy.hpp"
#include "AssertionFailure.hpp"
#include "timing.hpp"
#include "types.h"
#include "serialize.hpp"

//disable assert macro
#ifdef assert
#undef assert
#endif

namespace testsuite
{

using namespace util;

template<typename T>
inline void assert(const T& value, const T& expected, comparator::Comparator<T> comp)
{
    if (!comp->compare(value, expected))
    {
        throw AssertionFailure(
                std::string("expected ") + serialize(value) + " " + comp->assertion
                + " " + serialize(expected));
    }
}

template<typename T>
inline void assertException(test_function func)
{
    try
    {
        func();
    }
    catch (const T&)
    {
    }
    catch (const std::exception& e)
    {
        throw AssertionFailure(
                std::string("Wrong exception thrown, caught: ") + typeid(e).name());
    }
    catch (...)
    {
        throw AssertionFailure("Wrong exception thrown, caught: Unknown");
    }
}

inline void assertPerformance(test_function func, double maxTime)
{
    try
    {
        timing::duration dur;
        func();
        double dur_ms = (double) dur.get() / 1000.0;
        if (dur_ms > maxTime)
        {
            throw AssertionFailure(std::string("runtime > ") + serialize(maxTime));
        }
    }
    catch (const std::exception& e)
    {
        throw AssertionFailure(e.what());
    }
    catch (...)
    {
        throw AssertionFailure("Unknown exception thrown");
    }
}

}  // namespace testsuite

#endif /* SRC_UTIL_ASSERT_HPP_ */
