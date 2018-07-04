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

#ifndef TESTSUITE_TESTSTATS_HPP_
#define TESTSUITE_TESTSTATS_HPP_

#include <cstddef>

namespace sctf
{
namespace test
{
/**
 * Statistics class for testsuites.
 */
class TestStats
{
public:
    /**
     * c'tor
     */
    TestStats()
    {}

    /**
     * d'tor
     */
    virtual ~TestStats() noexcept
    {}

    inline std::size_t getNumTests() const
    {
        return num_of_tests;
    }

    inline std::size_t getNumFails() const
    {
        return num_of_fails;
    }

    inline std::size_t getNumErrs() const
    {
        return num_of_errs;
    }

private:
    /**
     * Only TestSuite is allowed to write these members.
     */
    friend class TestSuite;

    /**
     * Number of tests run
     */
    std::size_t num_of_tests = 0;

    /**
     * Number of failed tests.
     */
    std::size_t num_of_fails = 0;

    /**
     * Number of erroneous tests.
     */
    std::size_t num_of_errs = 0;
};
}
}  // testsuite

#endif /* TESTSUITE_TESTSTATS_HPP_ */
