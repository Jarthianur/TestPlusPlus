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


#ifndef SRC_UTIL_ASSERTIONFAILURE_HPP_
#define SRC_UTIL_ASSERTIONFAILURE_HPP_

#include <exception>
#include <string>

namespace testsuite
{

/**
 * Exception indicating an assertion has failed.
 */
class AssertionFailure: public std::exception
{
public:
    /**
     * c'tor with error msg.
     */
    inline AssertionFailure(const std::string& msg)
            : std::exception(),
              msg(msg)
    {
    }

    /**
     * d'tor
     */
    inline ~AssertionFailure() noexcept
    {
    }

    /**
     * Return err msg.
     */
    inline virtual const char* what() const noexcept override
    {
        return msg.c_str();
    }

private:
    /**
     * Error msg
     */
    const std::string msg;
};

}  // namespace testsuite

#endif /* SRC_UTIL_ASSERTIONFAILURE_HPP_ */
