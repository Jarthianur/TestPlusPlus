/*
 * duration.hpp
 *
 *  Created on: 15.03.2017
 *      Author: julian
 */

#ifndef SRC_UTIL_TIMING_HPP_
#define SRC_UTIL_TIMING_HPP_

#include <chrono>
#include <cstdint>

namespace testsuite
{

namespace util
{

namespace timing
{

struct duration final
{
    inline duration()
            : start(std::chrono::high_resolution_clock::now())
    {
    }

    inline std::uint64_t get()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
    }

    std::chrono::high_resolution_clock::time_point start;
};

}  // namespace timing

}  // namespace util

}  // namespace testsuite

#endif /* SRC_UTIL_TIMING_HPP_ */
