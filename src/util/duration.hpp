/*
 * duration.hpp
 *
 *  Created on: 15.03.2017
 *      Author: julian
 */

#ifndef SRC_UTIL_DURATION_HPP_
#define SRC_UTIL_DURATION_HPP_

#include <chrono>
#include <cstdint>

namespace testsuite
{

namespace util
{

struct duration final
{
    inline duration()
            : start(std::chrono::steady_clock::now())
    {
    }

    inline double get()
    {
        return std::chrono::duration<double, std::milli>(
                std::chrono::steady_clock::now() - start).count();
    }

    std::chrono::steady_clock::time_point start;
};

}  // namespace util

}  // namespace testsuite

#endif /* SRC_UTIL_DURATION_HPP_ */
