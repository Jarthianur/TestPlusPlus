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

#ifndef TEST_REFLEXIVE_TESTS_H_
#define TEST_REFLEXIVE_TESTS_H_

#define SCTF_EXTERN_EPSILON
//#define SCTF_EPSILON 0.000001

#include "sctf.hpp"

void reflexive_tests();

TEST_MODULE(test_sequential_module, { test("test", [] { assertTrue(true); }); })

TEST_MODULE_PARALLEL(test_parallel_module, {
    test("1", [] { assertTrue(true); });
    test("2", [] { assertTrue(true); });
})

#endif  // TEST_REFLEXIVE_TESTS_H_
