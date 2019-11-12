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
    describe_parallel("comparators")
        ->test("equals",
               [] {
                   ASSERT_FALSE(!equals(1, 1));
                   ASSERT_FALSE(!equals(equal_comparable(), equal_comparable()));
                   ASSERT_FALSE(!equals(unequal_comparable(false), unequal_comparable(false)));
                   ASSERT_FALSE(!equals(1.0, 1.0));
                   ASSERT_FALSE(!equals(1.0f, 1.0f));
                   ASSERT_TRUE(!equals(2, 1));
                   ASSERT_TRUE(!equals(equal_comparable(false), equal_comparable()));
                   ASSERT_TRUE(!equals(unequal_comparable(true), unequal_comparable(false)));
                   ASSERT_TRUE(!equals(1.1, 2.0));
                   ASSERT_TRUE(!equals(1.1f, 2.0f));
                   comparison c = equals(1, 2);
                   ASSERT_TRUE(!c);
                   ASSERT_T(*c, EQ, "Expected '1' to be equals '2'", std::string);
               })
        ->test("greater than",
               [] {
                   ASSERT_FALSE(!greater_than(2, 1));
                   ASSERT_FALSE(!greater_than(ordinal(), ordinal()));
                   ASSERT_FALSE(!greater_than(2.1, 1.9));
                   ASSERT_TRUE(!greater_than(1, 2));
                   ASSERT_TRUE(!greater_than(ordinal(false), ordinal()));
                   ASSERT_TRUE(!greater_than(2.1, 3.9));
                   comparison c = greater_than(1, 2);
                   ASSERT_TRUE(!c);
                   ASSERT_T(*c, EQ, "Expected '1' to be greater than '2'", std::string);
               })
        ->test("in range",
               [] {
                   ASSERT_FALSE(!in_range(1, std::vector<int>{1}));
                   ASSERT_FALSE(!in_range("a", std::string("a")));
                   ASSERT_TRUE(!in_range(2, std::vector<int>{1}));
                   ASSERT_TRUE(!in_range("b", std::string("a")));
               })
        ->test("less than",
               [] {
                   ASSERT_FALSE(!less_than(1, 2));
                   ASSERT_FALSE(!less_than(ordinal(), ordinal()));
                   ASSERT_FALSE(!less_than(1.9, 2.1));
                   ASSERT_TRUE(!less_than(2, 1));
                   ASSERT_TRUE(!less_than(ordinal(false), ordinal()));
                   ASSERT_TRUE(!less_than(3.9, 2.1));
                   comparison c = less_than(2, 1);
                   ASSERT_TRUE(!c);
                   ASSERT_T(*c, EQ, "Expected '2' to be less than '1'", std::string);
               })
        ->test("unequals", [] {
            ASSERT_FALSE(!unequals(1, 2));
            ASSERT_FALSE(!unequals(unequal_comparable(), unequal_comparable()));
            ASSERT_FALSE(!unequals(equal_comparable(false), equal_comparable(false)));
            ASSERT_FALSE(!unequals(1.0, 1.1));
            ASSERT_FALSE(!unequals(1.0f, 1.1f));
            ASSERT_TRUE(!unequals(2, 2));
            ASSERT_TRUE(!unequals(equal_comparable(), equal_comparable()));
            ASSERT_TRUE(!unequals(unequal_comparable(false), unequal_comparable()));
            ASSERT_TRUE(!unequals(2.0, 2.0));
            ASSERT_TRUE(!unequals(1.1f, 1.1f));
            comparison c = unequals(1, 1);
            ASSERT_TRUE(!c);
            ASSERT_T(*c, EQ, "Expected '1' to be unequals '1'", std::string);
        });

    describe<testsuite_parallel>("testsuite_parallel")->test("parallel run", [] {
        testsuite_shared ts = testsuite_parallel::create("ts", "ctx");
        ts->test("", [] { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
        ts->test("", [] { ASSERT_TRUE(false); });
        ts->test("", [] { throw std::logic_error(""); });
        ts->test("", [] { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
        ts->test("", [] { ASSERT_TRUE(false); });
        ts->test("", [] { throw std::logic_error(""); });
#ifdef _OPENMP
        int c = 200;
        /* workaround: see https://github.com/Jarthianur/simple-cpp-test-framework/issues/25 for
           details */
#    ifdef __clang__
        c = 250;
#    endif
        ASSERT_PERFORMANCE(ts->run(), c);
        ASSERT(ts->time(), LT, c);
#else
        ts->run();
        double t = 0.0;
        for ( auto const& tc : ts->testcases())
        {
         t += tc.duration();
        }
        ASSERT_EQUALS(ts->time(), t);
#endif
        statistics const& stat = ts->statistics();
        ASSERT_EQUALS(stat.tests(), 6);
        ASSERT_EQUALS(stat.errors(), 2);
        ASSERT_EQUALS(stat.failures(), 2);
        ASSERT_EQUALS(stat.successes(), 2);
    });

    describe<testsuite>("testsuite")
        ->test("creation",
               [] {
                   auto a = std::chrono::system_clock::now();
                   std::this_thread::sleep_for(std::chrono::seconds(1));
                   testsuite_shared ts = testsuite::create("ts", "ctx");
                   ASSERT(ts->timestamp(), GT, a);
                   ASSERT_T(ts->name(), EQ, "ts", std::string);
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
                   testcase const& tc1 = ts->testcases().at(0);
                   testcase const& tc2 = ts->testcases().at(1);
                   testcase const& tc3 = ts->testcases().at(2);
                   ASSERT_T(tc1.context(), EQ, "ctx", std::string);
                   ASSERT_T(tc2.context(), EQ, "ctx2", std::string);
                   ASSERT_T(tc3.context(), EQ, "int", std::string);
                   ts->run();
                   ASSERT_EQUALS(i, 1);
               })
        ->test("running", [] {
            testsuite_shared ts = testsuite::create("ts", "ctx");
            ts->test("", [] {});
            ts->test("", [] { ASSERT_TRUE(false); });
            ts->test("", [] { throw std::logic_error(""); });
            ts->run();
            statistics const& stat = ts->statistics();
            ASSERT_EQUALS(stat.tests(), 3);
            ASSERT_EQUALS(stat.errors(), 1);
            ASSERT_EQUALS(stat.failures(), 1);
            ASSERT_EQUALS(stat.successes(), 1);
            ts->run();
            ASSERT_EQUALS(stat.tests(), 3);
            ASSERT_EQUALS(stat.errors(), 1);
            ASSERT_EQUALS(stat.failures(), 1);
            ASSERT_EQUALS(stat.successes(), 1);
            ts->test("", [] {});
            ts->run();
            ASSERT_EQUALS(stat.tests(), 4);
            ASSERT_EQUALS(stat.successes(), 2);
            double t = 0.0;
            for (auto const& tc : ts->testcases())
            {
                t += tc.duration();
            }
            ASSERT_EQUALS(t, ts->time());
        });

    describe<testcase>("testcase")
        ->test("creation",
               [] {
                   testcase tc("t1", "ctx", [] {});
                   testcase tc2("t2", "", [] {});
                   ASSERT_EQUALS(tc.state(), testcase::result::NONE);
                   ASSERT_T(tc.context(), EQ, "ctx", std::string);
                   ASSERT_T(tc2.context(), EQ, "", std::string);
                   ASSERT_T(tc.name(), EQ, "t1", std::string);
               })
        ->test("successful execution",
               [] {
                   testcase tc("t1", "ctx", [] {});
                   tc();
                   ASSERT_EQUALS(tc.state(), testcase::result::PASSED);
                   ASSERT(tc.duration(), GT, 0.0);
                   ASSERT_ZERO(tc.err_msg().size());
               })
        ->test("failed execution",
               [] {
                   testcase tc("t1", "ctx", [] { ASSERT_TRUE(false); });
                   tc();
                   ASSERT_EQUALS(tc.state(), testcase::result::FAILED);
                   ASSERT(tc.duration(), GT, 0.0);
               })
        ->test("erroneous execution", [] {
            testcase tc("t1", "ctx", [] { throw std::logic_error("err"); });
            tc();
            ASSERT_EQUALS(tc.state(), testcase::result::ERROR);
            ASSERT(tc.duration(), GT, 0.0);
            ASSERT_T(tc.err_msg(), EQ, "err", std::string);

            testcase tc2("t2", "ctx", [] { throw 1; });
            tc2();
            ASSERT_EQUALS(tc2.state(), testcase::result::ERROR);
            ASSERT(tc2.duration(), GT, 0.0);
            ASSERT_T(tc2.err_msg(), EQ, "unknown error", std::string);
        });

    describe_parallel("stringify")
        ->test("bool",
               [] {
                   ASSERT_T(to_string(true), EQ, "true", std::string);
                   ASSERT_T(to_string(false), EQ, "false", std::string);
               })

        ->test("std::pair",
               [] { ASSERT_T("pair<int,int>", IN, to_string(std::make_pair(1, 2)), std::string); })
        ->test("nullptr",
               [] {
                   ASSERT_T(to_string(nullptr), EQ, "0", std::string);
                   ASSERT_T(to_string(NULL), EQ, "0", std::string);
               })
        ->test("string/cstring",
               [] {
                   std::string str("cstring");
                   ASSERT_EQUALS(to_string(str), str);
                   ASSERT_EQUALS(to_string("cstring"), str);
                   char const* cstr = "cstring";
                   ASSERT_EQUALS(to_string(cstr), str);
               })
        ->test("floating-point",
               [] {
                   ASSERT_T("1.123", IN, to_string(1.123f), std::string);
                   ASSERT_T("1.123", IN, to_string(1.123), std::string);
               })
        ->test("not streamable",
               [] { ASSERT_T(to_string(not_streamable()), EQ, "not_streamable", std::string); })
        ->test("streamable", [] { ASSERT_T(to_string(1), EQ, "1", std::string); });

    describe_parallel("test traits")
        ->test("is_streamable",
               [] {
                   ASSERT_NOTHROW((throw_if_not_streamable<std::ostringstream, streamable>()));
                   ASSERT_THROWS((throw_if_not_streamable<std::ostringstream, void_type>()),
                                 std::logic_error);
                   ASSERT_THROWS((throw_if_not_streamable<std::ostringstream, not_streamable>()),
                                 std::logic_error);
               })
        ->test("is_iterable",
               [] {
                   ASSERT_NOTHROW((throw_if_not_iterable<iterable>()));
                   ASSERT_THROWS((throw_if_not_iterable<void_type>()), std::logic_error);
                   ASSERT_THROWS((throw_if_not_iterable<not_iterable>()), std::logic_error);
               })
        ->test("is_ordinal",
               [] {
                   ASSERT_NOTHROW((throw_if_not_ordinal<ordinal>()));
                   ASSERT_THROWS((throw_if_not_ordinal<void_type>()), std::logic_error);
                   ASSERT_THROWS((throw_if_not_ordinal<not_ordinal>()), std::logic_error);
               })
        ->test("is_equal_comparable",
               [] {
                   ASSERT_NOTHROW((throw_if_not_equal_comparable<equal_comparable>()));
                   ASSERT_THROWS((throw_if_not_equal_comparable<void_type>()), std::logic_error);
                   ASSERT_THROWS((throw_if_not_equal_comparable<not_equal_comparable>()),
                                 std::logic_error);
               })
        ->test("is_unequal_comparable", [] {
            ASSERT_NOTHROW((throw_if_not_unequal_comparable<unequal_comparable>()));
            ASSERT_THROWS((throw_if_not_unequal_comparable<void_type>()), std::logic_error);
            ASSERT_THROWS((throw_if_not_unequal_comparable<not_unequal_comparable>()),
                          std::logic_error);
        });

    describe_parallel("test assertions")
        ->test("ASSERT",
               [] {
                   // successful
                   ASSERT_NOTHROW(ASSERT(1, EQUALS, 1));
                   ASSERT_NOTHROW(ASSERT(true, EQUALS, true));
                   ASSERT_NOTHROW(ASSERT(1.5, LESS, 100.3));
                   ASSERT_NOTHROW(ASSERT("hello", NE, "world"));
                   ASSERT_NOTHROW(ASSERT(2, IN, (std::vector<int>{1, 3, 2})));
                   // failing
                   ASSERT_THROWS(ASSERT(2, EQUALS, 1), assertion_failure);
                   ASSERT_THROWS(ASSERT(false, EQUALS, true), assertion_failure);
                   ASSERT_THROWS(ASSERT(1002.5, LESS, 100.3), assertion_failure);
                   ASSERT_THROWS(ASSERT("hello", EQ, "world"), assertion_failure);
                   ASSERT_THROWS(ASSERT(2, IN, (std::vector<int>{1, 3})), assertion_failure);
               })
        ->test("ASSERT_T",
               [] {
                   // successful
                   ASSERT_NOTHROW(ASSERT_T(1, EQUALS, 1, unsigned int));
                   ASSERT_NOTHROW(ASSERT_T(true, EQUALS, true, bool));
                   ASSERT_NOTHROW(ASSERT_T(1.5, LESS, 100.3, double));
                   ASSERT_NOTHROW(ASSERT_T("hello", NE, "world", std::string));
                   // failing
                   ASSERT_THROWS(ASSERT_T(2, EQUALS, 1, unsigned int), assertion_failure);
                   ASSERT_THROWS(ASSERT_T(false, EQUALS, true, bool), assertion_failure);
                   ASSERT_THROWS(ASSERT_T(1002.5, LESS, 100.3, double), assertion_failure);
                   ASSERT_THROWS(ASSERT_T("hello", EQ, "world", std::string), assertion_failure);
               })
        ->test("ASSERT_EQUALS",
               [] {
                   // successful
                   ASSERT_NOTHROW(ASSERT_EQUALS(1, 1));
                   ASSERT_NOTHROW(ASSERT_EQUALS(true, true));
                   ASSERT_NOTHROW(ASSERT_EQUALS("", ""));
                   ASSERT_NOTHROW(ASSERT_EQUALS(1.1, 1.1));
                   // failing
                   ASSERT_THROWS(ASSERT_EQUALS(1, 2), assertion_failure);
                   ASSERT_THROWS(ASSERT_EQUALS(false, true), assertion_failure);
                   ASSERT_THROWS(ASSERT_EQUALS("b", "a"), assertion_failure);
                   ASSERT_THROWS(ASSERT_EQUALS(1.2, 2.1), assertion_failure);
               })
        ->test("ASSERT_TRUE",
               [] {
                   // successful
                   ASSERT_NOTHROW(ASSERT_TRUE(true));
                   ASSERT_NOTHROW(ASSERT_TRUE(1 == 1));
                   // failing
                   ASSERT_THROWS(ASSERT_TRUE(false), assertion_failure);
                   ASSERT_THROWS(ASSERT_TRUE(1 == 2), assertion_failure);
               })
        ->test("ASSERT_FALSE",
               [] {
                   // successful
                   ASSERT_NOTHROW(ASSERT_FALSE(false));
                   ASSERT_NOTHROW(ASSERT_FALSE(1 == 2));
                   // failing
                   ASSERT_THROWS(ASSERT_FALSE(true), assertion_failure);
                   ASSERT_THROWS(ASSERT_FALSE(1 == 1), assertion_failure);
               })
        ->test("ASSERT_NOT_NULL",
               [] {
                   // successful
                   int         i = 1;
                   double      d = 1.0;
                   char const* s = "";
                   ASSERT_NOTHROW(ASSERT_NOT_NULL(&i));
                   ASSERT_NOTHROW(ASSERT_NOT_NULL(&d));
                   ASSERT_NOTHROW(ASSERT_NOT_NULL(&s));
                   // failing
                   ASSERT_THROWS(ASSERT_NOT_NULL(nullptr), assertion_failure);
                   ASSERT_THROWS(ASSERT_NOT_NULL(NULL), assertion_failure);
               })
        ->test("ASSERT_NULL",
               [] {
                   // successful
                   ASSERT_NOTHROW(ASSERT_NULL(nullptr));
                   ASSERT_NOTHROW(ASSERT_NULL(NULL));
                   // failing
                   int         i = 1;
                   double      d = 1.0;
                   char const* s = "";
                   ASSERT_THROWS(ASSERT_NULL(&i), assertion_failure);
                   ASSERT_THROWS(ASSERT_NULL(&d), assertion_failure);
                   ASSERT_THROWS(ASSERT_NULL(&s), assertion_failure);
               })
        ->test("ASSERT_ZERO",
               [] {
                   // successful
                   ASSERT_NOTHROW(ASSERT_ZERO(0));
                   ASSERT_NOTHROW(ASSERT_ZERO(0.0));
                   // failing
                   ASSERT_THROWS(ASSERT_ZERO(1), assertion_failure);
                   ASSERT_THROWS(ASSERT_ZERO(0.1), assertion_failure);
               })
        ->test("ASSERT_THROWS",
               [] {
                   // successful
                   ASSERT_NOTHROW(ASSERT_THROWS(throw std::logic_error(""), std::logic_error));
                   // failing
                   ASSERT_THROWS(ASSERT_THROWS(return, std::logic_error), assertion_failure);
                   ASSERT_THROWS(ASSERT_THROWS(throw std::runtime_error(""), std::logic_error),
                                 assertion_failure);
                   ASSERT_THROWS(ASSERT_THROWS(throw 1, std::logic_error), assertion_failure);
               })
        ->test("ASSERT_NOTHROW",
               [] {
                   // successful
                   ASSERT_NOTHROW(ASSERT_NOTHROW(return ));
                   // failing
                   ASSERT_THROWS(ASSERT_NOTHROW(throw std::runtime_error("")), assertion_failure);
                   ASSERT_THROWS(ASSERT_NOTHROW(throw 1), assertion_failure);
               })
        ->test("ASSERT_PERFORMANCE", [] {
            // successful
            ASSERT_NOTHROW(ASSERT_PERFORMANCE(return, 100));
            // failing
            ASSERT_THROWS(
                ASSERT_PERFORMANCE(std::this_thread::sleep_for(std::chrono::milliseconds(100)), 10),
                assertion_failure);
            ASSERT_THROWS(ASSERT_PERFORMANCE(throw 1, 100), assertion_failure);
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
                    auto const& tc = ts->testcases().at(i);
                    ASSERT_EQUALS(tc.cout(), std::string("out from ") + to_string(i + 1));
                    ASSERT_EQUALS(tc.cerr(), std::string("err from ") + to_string(i + 1));
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
                auto const& tc = ts->testcases().at(i);
                ASSERT_EQUALS(tc.cout(), std::string("out from ") + to_string(i + 1));
                ASSERT_EQUALS(tc.cerr(), std::string("err from ") + to_string(i + 1));
            }
        });
}
