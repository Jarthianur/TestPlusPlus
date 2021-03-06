/*
    Copyright (C) 2017  Jarthianur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TEST_BASIC_TESTS_H_
#define TEST_BASIC_TESTS_H_

#include "tpp.hpp"

void
basic_tests();

class Failure : public std::exception
{
public:
    explicit Failure(char const* msg_) : std::exception(), m_msg(msg_) {}

    auto
    what() const noexcept -> char const* override {
        return m_msg;
    }

private:
    char const* m_msg;
};

#endif  // TEST_BASIC_TESTS_H_
