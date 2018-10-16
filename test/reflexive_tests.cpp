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

#include "reflexive_tests.h"

#include <chrono>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

using namespace sctf;
using namespace util;

void reflexive_tests(test::TestSuitesRunner& runner)
{
    describe("test assertions", runner)
        ->test(
            "assert",
            [] {
                // successful
                assertNoExcept(assert(1, EQUALS, 1));
                assertNoExcept(assert(true, EQUALS, true));
                assertNoExcept(assert(1.5, LESS, 100.3));
                assertNoExcept(assert(2, IN, (interval<int>{1, 3})));
                assertNoExcept(assert("hello", NE, "world"));
                assertNoExcept(assert(2, IN, (std::vector<int>{1, 3, 2})));
                // failing
                assertException(assert(2, EQUALS, 1), AssertionFailure);
                assertException(assert(false, EQUALS, true), AssertionFailure);
                assertException(assert(1002.5, LESS, 100.3), AssertionFailure);
                assertException(assert(4, IN, (interval<int>{1, 3})), AssertionFailure);
                assertException(assert("hello", EQ, "world"), AssertionFailure);
                assertException(assert(2, IN, (std::vector<int>{1, 3})),
                                AssertionFailure);
            })
        ->test("assertT",
               [] {
                   // successful
                   assertNoExcept(assertT(1, EQUALS, 1, unsigned int));
                   assertNoExcept(assertT(true, EQUALS, true, bool));
                   assertNoExcept(assertT(1.5, LESS, 100.3, double));
                   assertNoExcept(assertT("hello", NE, "world", std::string));
                   // failing
                   assertException(assertT(2, EQUALS, 1, unsigned int), AssertionFailure);
                   assertException(assertT(false, EQUALS, true, bool), AssertionFailure);
                   assertException(assertT(1002.5, LESS, 100.3, double),
                                   AssertionFailure);
                   assertException(assertT("hello", EQ, "world", std::string),
                                   AssertionFailure);
               })
        ->test("assertEquals",
               [] {
                   // successful
                   assertNoExcept(assertEquals(1, 1));
                   assertNoExcept(assertEquals(true, true));
                   assertNoExcept(assertEquals("", ""));
                   assertNoExcept(assertEquals(1.1, 1.1));
                   // failing
                   assertException(assertEquals(1, 2), AssertionFailure);
                   assertException(assertEquals(false, true), AssertionFailure);
                   assertException(assertEquals("b", "a"), AssertionFailure);
                   assertException(assertEquals(1.2, 2.1), AssertionFailure);
               })
        ->test("assertInInterval",
               [] {
                   // successful
                   assertNoExcept(assertInInterval(1, 1, 2));
                   assertNoExcept(assertInInterval(1.0, 1.0, 2.0));
                   // failing
                   assertException(assertInInterval(1, 2, 3), AssertionFailure);
                   assertException(assertInInterval(1.0, 2.0, 3.3), AssertionFailure);
               })
        ->test("assertTrue",
               [] {
                   // successful
                   assertNoExcept(assertTrue(true));
                   assertNoExcept(assertTrue(1 == 1));
                   // failing
                   assertException(assertTrue(false), AssertionFailure);
                   assertException(assertTrue(1 == 2), AssertionFailure);
               })
        ->test("assertFalse",
               [] {
                   // successful
                   assertNoExcept(assertFalse(false));
                   assertNoExcept(assertFalse(1 == 2));
                   // failing
                   assertException(assertFalse(true), AssertionFailure);
                   assertException(assertFalse(1 == 1), AssertionFailure);
               })
        ->test("assertNotNull",
               [] {
                   // successful
                   int i         = 1;
                   double d      = 1.0;
                   const char* s = "";
                   assertNoExcept(assertNotNull(&i));
                   assertNoExcept(assertNotNull(&d));
                   assertNoExcept(assertNotNull(&s));
                   // failing
                   assertException(assertNotNull(nullptr), AssertionFailure);
                   assertException(assertNotNull(NULL), AssertionFailure);
               })
        ->test("assertZero",
               [] {
                   // successful
                   assertNoExcept(assertZero(0));
                   assertNoExcept(assertZero(0.0));
                   // failing
                   assertException(assertZero(1), AssertionFailure);
                   assertException(assertZero(0.1), AssertionFailure);
               })
        ->test("assertException",
               [] {
                   // successful
                   assertNoExcept(
                       assertException(throw std::logic_error(""), std::logic_error));
                   // failing
                   assertException(assertException(return, std::logic_error),
                                   AssertionFailure);
                   assertException(
                       assertException(throw std::runtime_error(""), std::logic_error),
                       AssertionFailure);
                   assertException(assertException(throw 1, std::logic_error),
                                   AssertionFailure);
               })
        ->test("assertNoExcept",
               [] {
                   // successful
                   assertNoExcept(assertNoExcept(return ));
                   // failing
                   assertException(assertNoExcept(throw std::runtime_error("")),
                                   AssertionFailure);
                   assertException(assertNoExcept(throw 1), AssertionFailure);
               })
        ->test("assertPerformance", [] {
            // successful
            assertNoExcept(assertPerformance(return, 100));
            // failing
            assertException(
                assertPerformance(
                    std::this_thread::sleep_for(std::chrono::milliseconds(100)), 10),
                AssertionFailure);
        });
}
