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
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "traits.hpp"

SCTF_SET_EPSILON(0.000001)

using namespace sctf;
using namespace _;

void reflexive_tests()
{
    describeParallel("comparators")
        ->test("equals",
               [] {
                   assertFalse(!equals(1, 1));
                   assertFalse(!equals(equal_comparable(), equal_comparable()));
                   assertFalse(!equals(unequal_comparable(false), unequal_comparable(false)));
                   assertFalse(!equals(1.0, 1.0));
                   assertFalse(!equals(1.0f, 1.0f));
                   assertTrue(!equals(2, 1));
                   assertTrue(!equals(equal_comparable(false), equal_comparable()));
                   assertTrue(!equals(unequal_comparable(true), unequal_comparable(false)));
                   assertTrue(!equals(1.1, 2.0));
                   assertTrue(!equals(1.1f, 2.0f));
                   comparison c = equals(1, 2);
                   assertTrue(!c);
                   assertT(*c, EQ, "Expected '1' to be equals '2'", std::string);
               })
        ->test("greater than",
               [] {
                   assertFalse(!greater_than(2, 1));
                   assertFalse(!greater_than(ordinal(), ordinal()));
                   assertFalse(!greater_than(2.1, 1.9));
                   assertTrue(!greater_than(1, 2));
                   assertTrue(!greater_than(ordinal(false), ordinal()));
                   assertTrue(!greater_than(2.1, 3.9));
                   comparison c = greater_than(1, 2);
                   assertTrue(!c);
                   assertT(*c, EQ, "Expected '1' to be greater than '2'", std::string);
               })
        ->test("in range",
               [] {
                   assertFalse(!in_range(1, std::vector<int>{1}));
                   assertFalse(!in_range("a", std::string("a")));
                   assertTrue(!in_range(2, std::vector<int>{1}));
                   assertTrue(!in_range("b", std::string("a")));
               })
        ->test("less than",
               [] {
                   assertFalse(!less_than(1, 2));
                   assertFalse(!less_than(ordinal(), ordinal()));
                   assertFalse(!less_than(1.9, 2.1));
                   assertTrue(!less_than(2, 1));
                   assertTrue(!less_than(ordinal(false), ordinal()));
                   assertTrue(!less_than(3.9, 2.1));
                   comparison c = less_than(2, 1);
                   assertTrue(!c);
                   assertT(*c, EQ, "Expected '2' to be less than '1'", std::string);
               })
        ->test("unequals", [] {
            assertFalse(!unequals(1, 2));
            assertFalse(!unequals(unequal_comparable(), unequal_comparable()));
            assertFalse(!unequals(equal_comparable(false), equal_comparable(false)));
            assertFalse(!unequals(1.0, 1.1));
            assertFalse(!unequals(1.0f, 1.1f));
            assertTrue(!unequals(2, 2));
            assertTrue(!unequals(equal_comparable(), equal_comparable()));
            assertTrue(!unequals(unequal_comparable(false), unequal_comparable()));
            assertTrue(!unequals(2.0, 2.0));
            assertTrue(!unequals(1.1f, 1.1f));
            comparison c = unequals(1, 1);
            assertTrue(!c);
            assertT(*c, EQ, "Expected '1' to be unequals '1'", std::string);
        });

    describe<testsuite_parallel>("TestSuiteParallel")->test("parallel run", [] {
        testsuite_shared ts = testsuite_parallel::create("ts", "ctx");
        ts->test("", [] { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
        ts->test("", [] { assertTrue(false); });
        ts->test("", [] { throw std::logic_error(""); });
        ts->test("", [] { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
        ts->test("", [] { assertTrue(false); });
        ts->test("", [] { throw std::logic_error(""); });
#ifdef _OPENMP
        assertPerformance(ts->run(), 200);
        assert(ts->time(), LT, 200);
#else
        ts->run();
        double t = 0.0;
        for (const auto& tc : ts->testcases())
        {
         t += tc.duration();
        }
        assertEquals(ts->time(), t);
#endif
        const statistics& stat = ts->statistics();
        assertEquals(stat.tests(), 6);
        assertEquals(stat.errors(), 2);
        assertEquals(stat.failures(), 2);
        assertEquals(stat.successes(), 2);
    });

    describe<testsuite>("TestSuite")
        ->test("creation",
               [] {
                   auto a = std::chrono::system_clock::now();
                   std::this_thread::sleep_for(std::chrono::seconds(1));
                   testsuite_shared ts = testsuite::create("ts", "ctx");
                   assert(ts->timestamp(), GT, a);
                   assertT(ts->name(), EQ, "ts", std::string);
               })
        ->test("meta functions",
               [] {
                   testsuite_shared ts = testsuite::create("ts", "ctx");
                   int              i  = 0;
                   ts->setup([&i] { i = 1; });
                   ts->after([&i] { ++i; });
                   ts->before([&i] { --i; });
                   ts->test("tc1", [] {});
                   ts->test("tc2", "ctx2", [] {});
                   ts->test<int>("tc3", [] {});
                   const testcase& tc1 = ts->testcases().at(0);
                   const testcase& tc2 = ts->testcases().at(1);
                   const testcase& tc3 = ts->testcases().at(2);
                   assertT(tc1.context(), EQ, "ctx", std::string);
                   assertT(tc2.context(), EQ, "ctx2", std::string);
                   assertT(tc3.context(), EQ, "int", std::string);
                   ts->run();
                   assertEquals(i, 1);
               })
        ->test("running", [] {
            testsuite_shared ts = testsuite::create("ts", "ctx");
            ts->test("", [] {});
            ts->test("", [] { assertTrue(false); });
            ts->test("", [] { throw std::logic_error(""); });
            ts->run();
            const statistics& stat = ts->statistics();
            assertEquals(stat.tests(), 3);
            assertEquals(stat.errors(), 1);
            assertEquals(stat.failures(), 1);
            assertEquals(stat.successes(), 1);
            ts->run();
            assertEquals(stat.tests(), 3);
            assertEquals(stat.errors(), 1);
            assertEquals(stat.failures(), 1);
            assertEquals(stat.successes(), 1);
            ts->test("", [] {});
            ts->run();
            assertEquals(stat.tests(), 4);
            assertEquals(stat.successes(), 2);
            double t = 0.0;
            for (const auto& tc : ts->testcases())
            {
                t += tc.duration();
            }
            assertEquals(t, ts->time());
        });

    describe<testcase>("TestCase")
        ->test("creation",
               [] {
                   testcase tc("t1", "ctx", [] {});
                   testcase tc2("t2", "", [] {});
                   assertEquals(tc.state(), testcase::result::NONE);
                   assertT(tc.context(), EQ, "ctx", std::string);
                   assertT(tc2.context(), EQ, "", std::string);
                   assertT(tc.name(), EQ, "t1", std::string);
               })
        ->test("successful execution",
               [] {
                   testcase tc("t1", "ctx", [] {});
                   tc();
                   assertEquals(tc.state(), testcase::result::PASSED);
                   assert(tc.duration(), GT, 0.0);
                   assertZero(tc.err_msg().size());
               })
        ->test("failed execution",
               [] {
                   testcase tc("t1", "ctx", [] { assertTrue(false); });
                   tc();
                   assertEquals(tc.state(), testcase::result::FAILED);
                   assert(tc.duration(), GT, 0.0);
               })
        ->test("erroneous execution", [] {
            testcase tc("t1", "ctx", [] { throw std::logic_error("err"); });
            tc();
            assertEquals(tc.state(), testcase::result::ERROR);
            assert(tc.duration(), GT, 0.0);
            assertT(tc.err_msg(), EQ, "err", std::string);

            testcase tc2("t2", "ctx", [] { throw 1; });
            tc2();
            assertEquals(tc2.state(), testcase::result::ERROR);
            assert(tc2.duration(), GT, 0.0);
            assertT(tc2.err_msg(), EQ, "unknown error", std::string);
        });

    describeParallel("serialize")
        ->test("bool",
               [] {
                   assertT(to_string(true), EQ, "true", std::string);
                   assertT(to_string(false), EQ, "false", std::string);
               })

        ->test("std::pair",
               [] { assertT("pair<int,int>", IN, to_string(std::make_pair(1, 2)), std::string); })
        ->test("nullptr",
               [] {
                   assertT(to_string(nullptr), EQ, "0", std::string);
                   assertT(to_string(NULL), EQ, "0", std::string);
               })
        ->test("string/cstring",
               [] {
                   std::string str("cstring");
                   assertEquals(to_string(str), str);
                   assertEquals(to_string("cstring"), str);
                   const char* cstr = "cstring";
                   assertEquals(to_string(cstr), str);
               })
        ->test("floating-point",
               [] {
                   assertT("1.123", IN, to_string(1.123f), std::string);
                   assertT("1.123", IN, to_string(1.123), std::string);
               })
        ->test("not streamable",
               [] { assertT(to_string(not_streamable()), EQ, "not_streamable", std::string); })
        ->test("streamable", [] { assertT(to_string(1), EQ, "1", std::string); });

    describeParallel("test traits")
        ->test("is_streamable",
               [] {
                   assertNoExcept((throw_if_not_streamable<std::ostringstream, streamable>()));
                   assertException((throw_if_not_streamable<std::ostringstream, void_type>()),
                                   std::logic_error);
                   assertException((throw_if_not_streamable<std::ostringstream, not_streamable>()),
                                   std::logic_error);
               })
        ->test("is_iterable",
               [] {
                   assertNoExcept((throw_if_not_iterable<iterable>()));
                   assertException((throw_if_not_iterable<void_type>()), std::logic_error);
                   assertException((throw_if_not_iterable<not_iterable>()), std::logic_error);
               })
        ->test("is_ordinal",
               [] {
                   assertNoExcept((throw_if_not_ordinal<ordinal>()));
                   assertException((throw_if_not_ordinal<void_type>()), std::logic_error);
                   assertException((throw_if_not_ordinal<not_ordinal>()), std::logic_error);
               })
        ->test("is_equal_comparable",
               [] {
                   assertNoExcept((throw_if_not_equal_comparable<equal_comparable>()));
                   assertException((throw_if_not_equal_comparable<void_type>()), std::logic_error);
                   assertException((throw_if_not_equal_comparable<not_equal_comparable>()),
                                   std::logic_error);
               })
        ->test("is_unequal_comparable", [] {
            assertNoExcept((throw_if_not_unequal_comparable<unequal_comparable>()));
            assertException((throw_if_not_unequal_comparable<void_type>()), std::logic_error);
            assertException((throw_if_not_unequal_comparable<not_unequal_comparable>()),
                            std::logic_error);
        });

    describeParallel("test assertions")
        ->test("assert",
               [] {
                   // successful
                   assertNoExcept(assert(1, EQUALS, 1));
                   assertNoExcept(assert(true, EQUALS, true));
                   assertNoExcept(assert(1.5, LESS, 100.3));
                   assertNoExcept(assert("hello", NE, "world"));
                   assertNoExcept(assert(2, IN, (std::vector<int>{1, 3, 2})));
                   // failing
                   assertException(assert(2, EQUALS, 1), assertion_failure);
                   assertException(assert(false, EQUALS, true), assertion_failure);
                   assertException(assert(1002.5, LESS, 100.3), assertion_failure);
                   assertException(assert("hello", EQ, "world"), assertion_failure);
                   assertException(assert(2, IN, (std::vector<int>{1, 3})), assertion_failure);
               })
        ->test("assertT",
               [] {
                   // successful
                   assertNoExcept(assertT(1, EQUALS, 1, unsigned int));
                   assertNoExcept(assertT(true, EQUALS, true, bool));
                   assertNoExcept(assertT(1.5, LESS, 100.3, double));
                   assertNoExcept(assertT("hello", NE, "world", std::string));
                   // failing
                   assertException(assertT(2, EQUALS, 1, unsigned int), assertion_failure);
                   assertException(assertT(false, EQUALS, true, bool), assertion_failure);
                   assertException(assertT(1002.5, LESS, 100.3, double), assertion_failure);
                   assertException(assertT("hello", EQ, "world", std::string), assertion_failure);
               })
        ->test("assertEquals",
               [] {
                   // successful
                   assertNoExcept(assertEquals(1, 1));
                   assertNoExcept(assertEquals(true, true));
                   assertNoExcept(assertEquals("", ""));
                   assertNoExcept(assertEquals(1.1, 1.1));
                   // failing
                   assertException(assertEquals(1, 2), assertion_failure);
                   assertException(assertEquals(false, true), assertion_failure);
                   assertException(assertEquals("b", "a"), assertion_failure);
                   assertException(assertEquals(1.2, 2.1), assertion_failure);
               })
        ->test("assertTrue",
               [] {
                   // successful
                   assertNoExcept(assertTrue(true));
                   assertNoExcept(assertTrue(1 == 1));
                   // failing
                   assertException(assertTrue(false), assertion_failure);
                   assertException(assertTrue(1 == 2), assertion_failure);
               })
        ->test("assertFalse",
               [] {
                   // successful
                   assertNoExcept(assertFalse(false));
                   assertNoExcept(assertFalse(1 == 2));
                   // failing
                   assertException(assertFalse(true), assertion_failure);
                   assertException(assertFalse(1 == 1), assertion_failure);
               })
        ->test("assertNotNull",
               [] {
                   // successful
                   int         i = 1;
                   double      d = 1.0;
                   const char* s = "";
                   assertNoExcept(assertNotNull(&i));
                   assertNoExcept(assertNotNull(&d));
                   assertNoExcept(assertNotNull(&s));
                   // failing
                   assertException(assertNotNull(nullptr), assertion_failure);
                   assertException(assertNotNull(NULL), assertion_failure);
               })
        ->test("assertZero",
               [] {
                   // successful
                   assertNoExcept(assertZero(0));
                   assertNoExcept(assertZero(0.0));
                   // failing
                   assertException(assertZero(1), assertion_failure);
                   assertException(assertZero(0.1), assertion_failure);
               })
        ->test("assertException",
               [] {
                   // successful
                   assertNoExcept(assertException(throw std::logic_error(""), std::logic_error));
                   // failing
                   assertException(assertException(return, std::logic_error), assertion_failure);
                   assertException(assertException(throw std::runtime_error(""), std::logic_error),
                                   assertion_failure);
                   assertException(assertException(throw 1, std::logic_error), assertion_failure);
               })
        ->test("assertNoExcept",
               [] {
                   // successful
                   assertNoExcept(assertNoExcept(return ));
                   // failing
                   assertException(assertNoExcept(throw std::runtime_error("")), assertion_failure);
                   assertException(assertNoExcept(throw 1), assertion_failure);
               })
        ->test("assertPerformance", [] {
            // successful
            assertNoExcept(assertPerformance(return, 100));
            // failing
            assertException(
                assertPerformance(std::this_thread::sleep_for(std::chrono::milliseconds(100)), 10),
                assertion_failure);
            assertException(assertPerformance(throw 1, 100), assertion_failure);
        });

    describe<testsuite>("test output capture")
        ->test<_::streambuf_proxy>(
            "single thread",
            [] {
                auto ts = testsuite::create("ts", "ctx");
                for (int i = 1; i < 9; ++i)
                {
                    ts->test("capture", [i] {
                        std::cout << 'o' << "ut from " << i;
                        std::cerr << 'e' << "rr from " << i;
                    });
                }
                ts->run();
                for (unsigned long i = 0; i < ts->testcases().size(); ++i)
                {
                    const auto& tc = ts->testcases().at(i);
                    assertEquals(tc.cout(), std::string("out from ") + to_string(i + 1));
                    assertEquals(tc.cerr(), std::string("err from ") + to_string(i + 1));
                }
            })
        ->test<_::streambuf_proxy_omp>("multi thread", [] {
            auto ts = testsuite_parallel::create("ts", "ctx");
            for (int i = 1; i < 9; ++i)
            {
                ts->test("capture", [i] {
                    std::cout << 'o' << "ut from " << i;
                    std::cerr << 'e' << "rr from " << i;
                });
            }
            ts->run();
            for (unsigned long i = 0; i < ts->testcases().size(); ++i)
            {
                const auto& tc = ts->testcases().at(i);
                assertEquals(tc.cout(), std::string("out from ") + to_string(i + 1));
                assertEquals(tc.cerr(), std::string("err from ") + to_string(i + 1));
            }
        });
}
