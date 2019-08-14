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

#include "sctf.hpp"

void basic_tests();

class Failure : public std::exception
{
public:
    Failure(const char* msg) : std::exception(), m_msg(msg) {}

    ~Failure() noexcept override = default;

    const char* what() const noexcept override
    {
        return m_msg;
    }

private:
    const char* m_msg;
};

#endif  // TEST_BASIC_TESTS_H_
