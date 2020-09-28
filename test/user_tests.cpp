/*
    Copyright (C) 2017 Jarthianur

    This file is part of simple-cpp-test-framework.

    simple-cpp-test-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple-cpp-test-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple-cpp-test-framework.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "sctf.hpp"

using sctf::GT;
using sctf::LT;

class MyClass
{
    int    m_i = 0;
    double m_d = 1.0;

public:
    void
    incr() {
        m_i += 1;
    }
    void
    mult(double m) {
        m_d *= m;
    }

    auto
    i() const -> int {
        return m_i;
    }
    auto
    d() const -> double {
        return m_d;
    }
};

DESCRIBE_PAR("TestMyClass") {
    MyClass fixture_;

    BEFORE_EACH() {
        fixture_.incr();
    };
    AFTER_EACH() {
        fixture_.mult(2.);
    };

    TEST("some test with i") {
        ASSERT(fixture_.i(), GT(), 0);
    };
    TEST("some test with d") {
        ASSERT(fixture_.d(), LT(), 5.0);
    };

    TEST("check epsilon") {
        std::cout << sctf::epsilon << std::flush;
    };
    TEST("some \"quoted\" name") {
        std::cerr << "print \"quoted\"" << std::endl;
    }
};
