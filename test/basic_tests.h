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

#ifndef TEST_BASIC_TESTS_H_
#define TEST_BASIC_TESTS_H_

#include <stdexcept>

#include "../sctf.hpp"
#include "../src/util/traits.hpp"

void basic_tests();

class Failure : public std::exception
{
public:
    Failure(const char* msg) : std::exception(), m_msg(msg)
    {}

    ~Failure() noexcept = default;

    const char* what() const noexcept override
    {
        return m_msg;
    }

private:
    const char* m_msg;
};

struct streamable
{
    template<typename S>
    friend auto operator<<(S& s, const streamable&) -> S&
    {
        return s;
    }
};

struct not_streamable
{
    template<typename S>
    friend auto operator<<(S& s, const not_streamable&) -> S& = delete;
};

template<
    typename S, typename T,
    typename std::enable_if<not sctf::util::is_streamable<S, T>::value>::type* = nullptr>
void throw_if_not_streamable()
{
    throw std::logic_error("Given type is not streamable");
}

template<typename S, typename T,
         typename std::enable_if<sctf::util::is_streamable<S, T>::value>::type* = nullptr>
void throw_if_not_streamable()
{}

#endif  // TEST_BASIC_TESTS_H_
