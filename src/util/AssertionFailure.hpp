/*
 * AssertionException.hpp
 *
 *  Created on: 15.03.2017
 *      Author: julian
 */

#ifndef SRC_UTIL_ASSERTIONFAILURE_HPP_
#define SRC_UTIL_ASSERTIONFAILURE_HPP_

#include <exception>
#include <string>

namespace testsuite
{

class AssertionFailure: public std::exception
{
public:
    inline AssertionFailure(const std::string& msg)
            : std::exception(),
              msg(msg)
    {
    }
    inline ~AssertionFailure() noexcept
    {
    }

    inline virtual const char* what() const noexcept override
    {
        return msg.c_str();
    }

private:
    const std::string msg;
};

}  // namespace testsuite

#endif /* SRC_UTIL_ASSERTIONFAILURE_HPP_ */
