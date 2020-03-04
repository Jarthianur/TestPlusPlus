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
using namespace private_;

SUITE_PAR(test_comparators)
{
    TEST(equals)
    {
        ASSERT_FALSE(!equals()(1, 1));
        ASSERT_FALSE(!equals()(equal_comparable(), equal_comparable()));
        ASSERT_FALSE(!equals()(unequal_comparable(false), unequal_comparable(false)));
        ASSERT_FALSE(!equals()(1.0, 1.0));
        ASSERT_FALSE(!equals()(1.0f, 1.0f));
        ASSERT_TRUE(!equals()(2, 1));
        ASSERT_TRUE(!equals()(equal_comparable(false), equal_comparable()));
        ASSERT_TRUE(!equals()(unequal_comparable(true), unequal_comparable(false)));
        ASSERT_TRUE(!equals()(1.1, 2.0));
        ASSERT_TRUE(!equals()(1.1f, 2.0f));
        comparison c = equals()(1, 2);
        ASSERT_TRUE(!c);
        ASSERT(*c, EQ, std::string("Expected 1 to be equals 2"));
    };
    TEST(greater_than)
    {
        ASSERT_FALSE(!greater_than()(2, 1));
        ASSERT_FALSE(!greater_than()(ordinal(), ordinal()));
        ASSERT_FALSE(!greater_than()(2.1, 1.9));
        ASSERT_TRUE(!greater_than()(1, 2));
        ASSERT_TRUE(!greater_than()(ordinal(false), ordinal()));
        ASSERT_TRUE(!greater_than()(2.1, 3.9));
        comparison c = greater_than()(1, 2);
        ASSERT_TRUE(!c);
        ASSERT(*c, EQ, std::string("Expected 1 to be greater than 2"));
    };
    TEST(in_range)
    {
        ASSERT_FALSE(!(in_range()(1, std::vector<int>{1})));
        ASSERT_FALSE(!(in_range()("a", std::string("a"))));
        ASSERT_TRUE(!(in_range()(2, std::vector<int>{1})));
        ASSERT_TRUE(!(in_range()("b", std::string("a"))));
    };
    TEST(less_than)
    {
        ASSERT_FALSE(!less_than()(1, 2));
        ASSERT_FALSE(!less_than()(ordinal(), ordinal()));
        ASSERT_FALSE(!less_than()(1.9, 2.1));
        ASSERT_TRUE(!less_than()(2, 1));
        ASSERT_TRUE(!less_than()(ordinal(false), ordinal()));
        ASSERT_TRUE(!less_than()(3.9, 2.1));
        comparison c = less_than()(2, 1);
        ASSERT_TRUE(!c);
        ASSERT(*c, EQ, std::string("Expected 2 to be less than 1"));
    };
    TEST(unequals)
    {
        ASSERT_FALSE(!unequals()(1, 2));
        ASSERT_FALSE(!unequals()(unequal_comparable(), unequal_comparable()));
        ASSERT_FALSE(!unequals()(equal_comparable(false), equal_comparable(false)));
        ASSERT_FALSE(!unequals()(1.0, 1.1));
        ASSERT_FALSE(!unequals()(1.0f, 1.1f));
        ASSERT_TRUE(!unequals()(2, 2));
        ASSERT_TRUE(!unequals()(equal_comparable(), equal_comparable()));
        ASSERT_TRUE(!unequals()(unequal_comparable(false), unequal_comparable()));
        ASSERT_TRUE(!unequals()(2.0, 2.0));
        ASSERT_TRUE(!unequals()(1.1f, 1.1f));
        comparison c = unequals()(1, 1);
        ASSERT_TRUE(!c);
        ASSERT(*c, EQ, std::string("Expected 1 to be unequals 1"));
    };
};

SUITE(test_testsuite_parallel)
{
    TEST(parallel_run)
    {
        testsuite_ptr ts = testsuite_parallel::create("ts");
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
        ASSERT(ts->execution_time(), LT, c);
#else
        ts->run();
        double t = 0.0;
        for (auto const& tc : ts->testcases())
        {
            t += tc.duration();
        }
        ASSERT_EQ(ts->execution_time(), t);
#endif
        statistics const& stat = ts->statistics();
        ASSERT_EQ(stat.tests(), 6ul);
        ASSERT_EQ(stat.errors(), 2ul);
        ASSERT_EQ(stat.failures(), 2ul);
        ASSERT_EQ(stat.successes(), 2ul);
    };
};

SUITE(test_testsuite)
{
    TEST(creation)
    {
        auto a = std::chrono::system_clock::now();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        testsuite_ptr ts = testsuite::create("ts");
        ASSERT(ts->timestamp(), GT, a);
        ASSERT(ts->name(), EQ, std::string("ts"));
    };
    TEST(meta_functions)
    {
        testsuite_ptr ts = testsuite::create("ts");
        int           i  = 0;
        ts->setup([&i] { i = 1; });
        ts->after_each([&i] { ++i; });
        ts->before_each([&i] { --i; });
        ts->test("tc1", [] {});
        ts->test("tc2", [] {});
        ts->test("tc3", [] {});
        testcase const& tc1 = ts->testcases().at(0);
        testcase const& tc2 = ts->testcases().at(1);
        testcase const& tc3 = ts->testcases().at(2);
        ASSERT(tc1.context(), EQ, std::string("ts"));
        ASSERT(tc2.context(), EQ, std::string("ts"));
        ASSERT(tc3.context(), EQ, std::string("ts"));
        ts->run();
        ASSERT_EQ(i, 1);
    };
    TEST(running)
    {
        testsuite_ptr ts = testsuite::create("ts");
        ts->test("", [] {});
        ts->test("", [] { ASSERT_TRUE(false); });
        ts->test("", [] { throw std::logic_error(""); });
        ts->run();
        statistics const& stat = ts->statistics();
        ASSERT_EQ(stat.tests(), 3ul);
        ASSERT_EQ(stat.errors(), 1ul);
        ASSERT_EQ(stat.failures(), 1ul);
        ASSERT_EQ(stat.successes(), 1ul);
        ts->run();
        ASSERT_EQ(stat.tests(), 3ul);
        ASSERT_EQ(stat.errors(), 1ul);
        ASSERT_EQ(stat.failures(), 1ul);
        ASSERT_EQ(stat.successes(), 1ul);
        ts->test("", [] {});
        ts->run();
        ASSERT_EQ(stat.tests(), 4ul);
        ASSERT_EQ(stat.successes(), 2ul);
        double t = 0.0;
        for (auto const& tc : ts->testcases())
        {
            t += tc.duration();
        }
        ASSERT_EQ(t, ts->execution_time());
    };
};

SUITE(test_testcase)
{
    TEST(creation)
    {
        testcase tc("t1", "ctx", [] {});
        testcase tc2("t2", "", [] {});
        ASSERT_EQ(tc.state(), testcase::result::NONE);
        ASSERT(tc.context(), EQ, std::string("ctx"));
        ASSERT(tc2.context(), EQ, std::string(""));
        ASSERT(tc.name(), EQ, std::string("t1"));
    };
    TEST(successful_execution)
    {
        testcase tc("t1", "ctx", [] {});
        tc();
        ASSERT_EQ(tc.state(), testcase::result::PASSED);
        ASSERT(tc.duration(), GT, 0.0);
        ASSERT_ZERO(tc.err_msg().size());
    };
    TEST(failed_execution)
    {
        testcase tc("t1", "ctx", [] { ASSERT_TRUE(false); });
        tc();
        ASSERT_EQ(tc.state(), testcase::result::FAILED);
        ASSERT(tc.duration(), GT, 0.0);
    };
    TEST(erroneous_execution)
    {
        testcase tc("t1", "ctx", [] { throw std::logic_error("err"); });
        tc();
        ASSERT_EQ(tc.state(), testcase::result::ERROR);
        ASSERT(tc.duration(), GT, 0.0);
        ASSERT(tc.err_msg(), EQ, std::string("err"));

        testcase tc2("t2", "ctx", [] { throw 1; });
        tc2();
        ASSERT_EQ(tc2.state(), testcase::result::ERROR);
        ASSERT(tc2.duration(), GT, 0.0);
        ASSERT(tc2.err_msg(), EQ, std::string("unknown error"));
    };
};

SUITE_PAR(test_stringify)
{
    TEST(bool)
    {
        ASSERT(to_string(true), EQ, std::string("true"));
        ASSERT(to_string(false), EQ, std::string("false"));
    };
    TEST(std_pair)
    {
        ASSERT(std::string("pair<int,int>"), IN, to_string(std::make_pair(1, 2)));
    };
    TEST(nullptr)
    {
        ASSERT(to_string(nullptr), EQ, std::string("0"));
        ASSERT(to_string(NULL), EQ, std::string("0"));
    };
    TEST(string_cstring)
    {
        std::string str("string");
        ASSERT_EQ(to_string(str), "\"string\"");
        ASSERT_EQ(to_string("cstring"), "\"cstring\"");
        char const* cstr = "cstring";
        ASSERT_EQ(to_string(cstr), "\"cstring\"");
    };
    TEST(floating_point)
    {
        ASSERT(std::string("1.123"), IN, to_string(1.123f));
        ASSERT(std::string("1.123"), IN, to_string(1.123));
    };
    TEST(not_streamable)
    {
        ASSERT(to_string(not_streamable()), EQ, std::string("not_streamable"));
    };
    TEST(streamable)
    {
        ASSERT(to_string(1), EQ, std::string("1"));
    };
};

SUITE_PAR(test_traits)
{
    TEST(is_streamable)
    {
        ASSERT_NOTHROW((throw_if_not_streamable<std::ostringstream, streamable>()));
        ASSERT_THROWS((throw_if_not_streamable<std::ostringstream, void_type>()), std::logic_error);
        ASSERT_THROWS((throw_if_not_streamable<std::ostringstream, not_streamable>()),
                      std::logic_error);
    };
    TEST(is_iterable)
    {
        ASSERT_NOTHROW((throw_if_not_iterable<iterable>()));
        ASSERT_THROWS((throw_if_not_iterable<void_type>()), std::logic_error);
        ASSERT_THROWS((throw_if_not_iterable<not_iterable>()), std::logic_error);
    };
    TEST(is_ordinal)
    {
        ASSERT_NOTHROW((throw_if_not_ordinal<ordinal>()));
        ASSERT_THROWS((throw_if_not_ordinal<void_type>()), std::logic_error);
        ASSERT_THROWS((throw_if_not_ordinal<not_ordinal>()), std::logic_error);
    };
    TEST(is_equal_comparable)
    {
        ASSERT_NOTHROW((throw_if_not_equal_comparable<equal_comparable>()));
        ASSERT_THROWS((throw_if_not_equal_comparable<void_type>()), std::logic_error);
        ASSERT_THROWS((throw_if_not_equal_comparable<not_equal_comparable>()), std::logic_error);
    };
    TEST(is_unequal_comparable)
    {
        ASSERT_NOTHROW((throw_if_not_unequal_comparable<unequal_comparable>()));
        ASSERT_THROWS((throw_if_not_unequal_comparable<void_type>()), std::logic_error);
        ASSERT_THROWS((throw_if_not_unequal_comparable<not_unequal_comparable>()),
                      std::logic_error);
    };
};

SUITE_PAR(test_assertions)
{
    TEST(assert)
    {  // successful
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
    };
    TEST(assert_not)
    {
        // successful
        ASSERT_NOTHROW(ASSERT_NOT(1, EQUALS, 2));
        ASSERT_NOTHROW(ASSERT_NOT(true, EQUALS, false));
        ASSERT_NOTHROW(ASSERT_NOT(1.5, LESS, 0.3));
        ASSERT_NOTHROW(ASSERT_NOT("hello", NE, "hello"));
        // failing
        ASSERT_THROWS(ASSERT_NOT(2, EQUALS, 2), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT(false, EQUALS, false), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT(1002.4, LESS, 1002.5), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT("hello", EQ, "hello"), assertion_failure);
    };
    TEST(assert_equals)
    {
        // successful
        ASSERT_NOTHROW(ASSERT_EQ(1, 1));
        ASSERT_NOTHROW(ASSERT_EQ(true, true));
        ASSERT_NOTHROW(ASSERT_EQ("", ""));
        ASSERT_NOTHROW(ASSERT_EQ(1.1, 1.1));
        // failing
        ASSERT_THROWS(ASSERT_EQ(1, 2), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ(false, true), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ("b", "a"), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ(1.2, 2.1), assertion_failure);
    };
    TEST(assert_true)
    {
        // successful
        ASSERT_NOTHROW(ASSERT_TRUE(true));
        ASSERT_NOTHROW(ASSERT_TRUE(1 == 1));
        // failing
        ASSERT_THROWS(ASSERT_TRUE(false), assertion_failure);
        ASSERT_THROWS(ASSERT_TRUE(1 == 2), assertion_failure);
    };
    TEST(assert_false)
    {
        // successful
        ASSERT_NOTHROW(ASSERT_FALSE(false));
        ASSERT_NOTHROW(ASSERT_FALSE(1 == 2));
        // failing
        ASSERT_THROWS(ASSERT_FALSE(true), assertion_failure);
        ASSERT_THROWS(ASSERT_FALSE(1 == 1), assertion_failure);
    };
    TEST(assert_not_null)
    {
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
    };
    TEST(assert_null)
    {
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
    };
    TEST(assert_zero)
    {
        // successful
        ASSERT_NOTHROW(ASSERT_ZERO(0));
        ASSERT_NOTHROW(ASSERT_ZERO(0.0));
        // failing
        ASSERT_THROWS(ASSERT_ZERO(1), assertion_failure);
        ASSERT_THROWS(ASSERT_ZERO(0.1), assertion_failure);
    };
    TEST(assert_throws)
    {
        // successful
        ASSERT_NOTHROW(ASSERT_THROWS(throw std::logic_error(""), std::logic_error));
        // failing
        ASSERT_THROWS(ASSERT_THROWS(return, std::logic_error), assertion_failure);
        ASSERT_THROWS(ASSERT_THROWS(throw std::runtime_error(""), std::logic_error),
                      assertion_failure);
        ASSERT_THROWS(ASSERT_THROWS(throw 1, std::logic_error), assertion_failure);
    };
    TEST(assert_nothrow)
    {
        // successful
        ASSERT_NOTHROW(ASSERT_NOTHROW(return ));
        // failing
        ASSERT_THROWS(ASSERT_NOTHROW(throw std::runtime_error("")), assertion_failure);
        ASSERT_THROWS(ASSERT_NOTHROW(throw 1), assertion_failure);
    };
    TEST(assert_performance)
    {
        // successful
        ASSERT_NOTHROW(ASSERT_PERFORMANCE(return, 100));
        // failing
        ASSERT_THROWS(
            ASSERT_PERFORMANCE(std::this_thread::sleep_for(std::chrono::milliseconds(100)), 10),
            assertion_failure);
        ASSERT_THROWS(ASSERT_PERFORMANCE(throw 1, 100), assertion_failure);
    };
};

SUITE(test_output_capture)
{
    TEST(single_thread)
    {
        auto ts = testsuite::create("ts");
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
            ASSERT_EQ(tc.cout(), std::string("out from ") + to_string(i + 1));
            ASSERT_EQ(tc.cerr(), std::string("err from ") + to_string(i + 1));
        }
    };
    TEST(multi_thread)
    {
        auto ts = testsuite_parallel::create("ts");
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
            ASSERT_EQ(tc.cout(), std::string("out from ") + to_string(i + 1));
            ASSERT_EQ(tc.cerr(), std::string("err from ") + to_string(i + 1));
        }
    };
};
