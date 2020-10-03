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

#include <chrono>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "test_traits.hpp"
#include "tpp.hpp"

#ifdef TPP_INTERN_SYS_UNIX
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wunused-variable"
#    pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

using tpp::EQ;
using tpp::EQUALS;
using tpp::GT;
using tpp::IN;
using tpp::LESS;
using tpp::LIKE;
using tpp::LT;
using tpp::operator""_re;
using tpp::operator""_re_i;
using tpp::MATCH;
using tpp::intern::to_string;
using tpp::intern::compare::comparison;
using tpp::intern::test::assertion_failure;
using tpp::intern::test::statistic;
using tpp::intern::test::testcase;
using tpp::intern::test::testsuite;
using tpp::intern::test::testsuite_parallel;
using tpp::intern::test::testsuite_ptr;

SUITE_PAR("test_comparators") {
    TEST("equals") {
        // test for equals correctly returning true
        ASSERT_TRUE(EQ()('a', 'a'));
        ASSERT_FALSE(!EQ()('a', 'a'));
        ASSERT_FALSE(!EQ()(true, true));
        ASSERT_FALSE(!EQ()("a", "a"));
        ASSERT_FALSE(!EQ()(std::string("a"), "a"));
        ASSERT_FALSE(!EQ()(std::string("a"), std::string("a")));
        ASSERT_FALSE(!EQ()(std::vector<int>{1, 2}, std::vector<int>{1, 2}));
        ASSERT_FALSE(!EQ()(1, 1));
        ASSERT_FALSE(!EQ()(1.0, 1.0));
        ASSERT_FALSE(!EQ()(1.0F, 1.0F));

        ASSERT_TRUE(!(!EQ())(1, 1));
        ASSERT_TRUE(!(!EQ())(1.0, 1.0));
        ASSERT_TRUE(!(!EQ())(1.0F, 1.0F));
        ASSERT_TRUE(!EQ()(2, 1));
        ASSERT_TRUE(!EQ()(1.1, 2.0));
        ASSERT_TRUE(!EQ()(1.1F, 2.0F));
        ASSERT_FALSE(!(!EQ())(2, 1));
        ASSERT_FALSE(!(!EQ())(1.1, 2.0));
        ASSERT_FALSE(!(!EQ())(1.1F, 2.0F));
        comparison c = tpp::intern::compare::equals<void>()(1, 2);
        ASSERT_TRUE(!c);
        ASSERT(*c, EQ(), std::string("Expected 1 to be equals 2"));
    };
    TEST("greater_than") {
        ASSERT_FALSE(!GT()(2, 1));
        ASSERT_FALSE(!GT()(2.1, 1.9));
        ASSERT_TRUE(!(!GT())(2, 1));
        ASSERT_TRUE(!(!GT())(2.1, 1.9));
        ASSERT_TRUE(!GT()(1, 2));
        ASSERT_TRUE(!GT()(2.1, 3.9));
        ASSERT_FALSE(!(!GT())(1, 2));
        ASSERT_FALSE(!(!GT())(2.1, 3.9));
        comparison c = GT()(1, 2);
        ASSERT_TRUE(!c);
        ASSERT(*c, EQ(), std::string("Expected 1 to be greater than 2"));
    };
    TEST("in_range") {
        ASSERT_FALSE(!(IN()(1, std::vector<int>{1})));
        ASSERT_FALSE(!(IN()("a", std::string("a"))));
        ASSERT_TRUE(!((!IN())(1, std::vector<int>{1})));
        ASSERT_TRUE(!((!IN())("a", std::string("a"))));
        ASSERT_TRUE(!(IN()(2, std::vector<int>{1})));
        ASSERT_TRUE(!(IN()("b", std::string("a"))));
        ASSERT_FALSE(!((!IN())(2, std::vector<int>{1})));
        ASSERT_FALSE(!((!IN())("b", std::string("a"))));
    };
    TEST("less_than") {
        ASSERT_FALSE(!LT()(1, 2));
        ASSERT_FALSE(!LT()(1.9, 2.1));
        ASSERT_TRUE(!(!LT())(1, 2));
        ASSERT_TRUE(!(!LT())(1.9, 2.1));
        ASSERT_TRUE(!LT()(2, 1));
        ASSERT_TRUE(!LT()(3.9, 2.1));
        ASSERT_FALSE(!(!LT())(2, 1));
        ASSERT_FALSE(!(!LT())(3.9, 2.1));
        comparison c = LT()(2, 1);
        ASSERT_TRUE(!c);
        ASSERT(*c, EQ(), std::string("Expected 2 to be less than 1"));
    };
    TEST("match") {
        ASSERT_FALSE(!MATCH()("hello world", ".*"_re));
        ASSERT_FALSE(!MATCH()("hello world", "HELLO WORLD"_re_i));
        ASSERT_FALSE(!MATCH()("hello world", ".*"));
        ASSERT_FALSE(!MATCH()("hello world 11", "\\S{5}\\s.*?\\d+"_re));
        ASSERT_FALSE(!MATCH()(std::string("hello world"), ".*"_re));
        ASSERT_TRUE(!(!MATCH())("hello world", ".*"_re));
        ASSERT_TRUE(!(!MATCH())("hello world", "HELLO WORLD"_re_i));
        ASSERT_TRUE(!(!MATCH())("hello world", ".*"));
        ASSERT_TRUE(!(!MATCH())("hello world 11", "\\S{5}\\s.*?\\d+"_re));
        ASSERT_TRUE(!(!MATCH())(std::string("hello world"), ".*"_re));
        ASSERT_TRUE(!MATCH()("hello world", "\\s*"_re));
        ASSERT_TRUE(!MATCH()("hello world", "AAA"_re_i));
        ASSERT_TRUE(!MATCH()("hello world", "fff"));
        ASSERT_TRUE(!MATCH()("hello world 11", "\\S{7}\\s.*?\\d"_re));
        ASSERT_TRUE(!MATCH()(std::string("hello world"), "[+-]\\d+"_re));
        ASSERT_FALSE(!(!MATCH())("hello world", "\\s*"_re));
        ASSERT_FALSE(!(!MATCH())("hello world", "AAA"_re_i));
        ASSERT_FALSE(!(!MATCH())("hello world", "fff"));
        ASSERT_FALSE(!(!MATCH())("hello world 11", "\\S{7}\\s.*?\\d"_re));
        ASSERT_FALSE(!(!MATCH())(std::string("hello world"), "[+-]\\d+"_re));
        std::cmatch res;
        ASSERT_FALSE(!MATCH(res)("hello world 11", "(\\S{5})\\s(.*?)(\\d+)"_re));
        ASSERT_EQ(res.str(1), "hello");
        ASSERT_EQ(res.str(2), "world ");
        ASSERT_EQ(res.str(3), "11");
    };
    TEST("like") {
        ASSERT_FALSE(!LIKE()("hello world", "hell"_re));
        ASSERT_FALSE(!LIKE()("hello world", "HELL"_re_i));
        ASSERT_FALSE(!LIKE()("hello world", ".*?"));
        ASSERT_FALSE(!LIKE()("hello world 11", "\\S{5}"_re));
        ASSERT_FALSE(!LIKE()(std::string("hello world"), "hell"_re));
        ASSERT_TRUE(!(!LIKE())("hello world", "hell"_re));
        ASSERT_TRUE(!(!LIKE())("hello world", "HELL"_re_i));
        ASSERT_TRUE(!(!LIKE())("hello world", ".*?"));
        ASSERT_TRUE(!(!LIKE())("hello world 11", "\\S{5}"_re));
        ASSERT_TRUE(!(!LIKE())(std::string("hello world"), "hell"_re));
        ASSERT_TRUE(!LIKE()("hello world", "blub"_re));
        ASSERT_TRUE(!LIKE()("hello world", "AAA"_re_i));
        ASSERT_TRUE(!LIKE()("hello world 11", "\\S{7}"_re));
        ASSERT_TRUE(!LIKE()(std::string("hello world"), "[+-]\\d+"_re));
        ASSERT_FALSE(!(!LIKE())("hello world", "blub"_re));
        ASSERT_FALSE(!(!LIKE())("hello world", "AAA"_re_i));
        ASSERT_FALSE(!(!LIKE())("hello world 11", "\\S{7}"_re));
        ASSERT_FALSE(!(!LIKE())(std::string("hello world"), "[+-]\\d+"_re));
        std::cmatch res;
        ASSERT_FALSE(!LIKE(res)("hello world 11", ".*?(\\d+)"_re));
        ASSERT_EQ(res.str(1), "11");
    };
};

SUITE("test_testsuite_parallel") {
    TEST("parallel_run") {
        testsuite_ptr ts = testsuite_parallel::create("ts");
        ts->test("", [] { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
        ts->test("", [] { ASSERT_TRUE(false); });
        ts->test("", [] { throw std::logic_error(""); });
        ts->test("", [] { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
        ts->test("", [] { ASSERT_TRUE(false); });
        ts->test("", [] { throw std::logic_error(""); });
#ifdef _OPENMP
        int c = 200;
/* workaround: see https://github.com/Jarthianur/TestPlusPlus (Test++)/issues/25 for
   details */
#    ifdef __clang__
        c = 300;
#    endif
        ASSERT_RUNTIME(ts->run(), c);
        ASSERT(ts->statistics().elapsed_time(), LT(), c);
#else
        ts->run();
        double t = 0.0;
        for (auto const& tc : ts->testcases()) {
            t += tc.elapsed_time();
        }
        ASSERT_EQ(ts->statistics().elapsed_time(), t);
#endif
        statistic const& stat = ts->statistics();
        ASSERT_EQ(stat.tests(), 6UL);
        ASSERT_EQ(stat.errors(), 2UL);
        ASSERT_EQ(stat.failures(), 2UL);
        ASSERT_EQ(stat.successes(), 2UL);
    };
};

SUITE("test_testsuite") {
    TEST("creation") {
        auto a = std::chrono::system_clock::now();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        testsuite_ptr ts = testsuite::create("ts");
        ASSERT(ts->timestamp(), GT(), a);
        ASSERT(ts->name(), EQ(), std::string("ts"));
    };
    TEST("meta_functions") {
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
        ASSERT(tc1.suite_name(), EQ(), std::string("ts"));
        ASSERT(tc2.suite_name(), EQ(), std::string("ts"));
        ASSERT(tc3.suite_name(), EQ(), std::string("ts"));
        ts->run();
        ASSERT_EQ(i, 1);
    };
    TEST("running") {
        testsuite_ptr ts = testsuite::create("ts");
        ts->test("", [] {});
        ts->test("", [] { ASSERT_TRUE(false); });
        ts->test("", [] { throw std::logic_error(""); });
        ts->run();
        statistic const& stat = ts->statistics();
        ASSERT_EQ(stat.tests(), 3UL);
        ASSERT_EQ(stat.errors(), 1UL);
        ASSERT_EQ(stat.failures(), 1UL);
        ASSERT_EQ(stat.successes(), 1UL);
        ts->run();
        ASSERT_EQ(stat.tests(), 3UL);
        ASSERT_EQ(stat.errors(), 1UL);
        ASSERT_EQ(stat.failures(), 1UL);
        ASSERT_EQ(stat.successes(), 1UL);
        ts->test("", [] {});
        ts->run();
        ASSERT_EQ(stat.tests(), 4UL);
        ASSERT_EQ(stat.successes(), 2UL);
        double t = 0.0;
        for (auto const& tc : ts->testcases()) {
            t += tc.elapsed_time();
        }
        ASSERT_EQ(t, ts->statistics().elapsed_time());
    };
};

SUITE_PAR("test_testcase") {
    TEST("creation") {
        testcase tc({"t1", "ctx"}, [] {});
        testcase tc2({"t2", ""}, [] {});
        ASSERT_EQ(tc.result(), testcase::IS_UNDONE);
        ASSERT(tc.suite_name(), EQ(), std::string("ctx"));
        ASSERT(tc2.suite_name(), EQ(), std::string(""));
        ASSERT(tc.name(), EQ(), std::string("t1"));
    };
    TEST("successful_execution") {
        testcase tc({"t1", "ctx"}, [] {});
        tc();
        ASSERT_EQ(tc.result(), testcase::HAS_PASSED);
        ASSERT(tc.elapsed_time(), GT(), 0.0);
        ASSERT_EQ(tc.reason().size(), 0);
    };
    TEST("failed_execution") {
        testcase tc({"t1", "ctx"}, [] { ASSERT_TRUE(false); });
        tc();
        ASSERT_EQ(tc.result(), testcase::HAS_FAILED);
        ASSERT(tc.elapsed_time(), GT(), 0.0);
    };
    TEST("erroneous_execution") {
        testcase tc({"t1", "ctx"}, [] { throw std::logic_error("err"); });
        tc();
        ASSERT_EQ(tc.result(), testcase::HAD_ERROR);
        ASSERT(tc.elapsed_time(), GT(), 0.0);
        ASSERT(tc.reason(), EQ(), std::string("err"));

        testcase tc2({"t2", "ctx"}, [] { throw 1; });
        tc2();
        ASSERT_EQ(tc2.result(), testcase::HAD_ERROR);
        ASSERT(tc2.elapsed_time(), GT(), 0.0);
        ASSERT(tc2.reason(), EQ(), std::string("unknown error"));
    };
};

SUITE_PAR("test_stringify") {
    class base
    {
    public:
        virtual ~base() = default;
    };
    class derived : public base
    {};

    AFTER_EACH() {
        std::cout << std::flush;
    };

    TEST("bool") {
        ASSERT(to_string(true), EQ(), std::string("true"));
        ASSERT(to_string(false), EQ(), std::string("false"));
    };
    TEST("std_pair") {
        ASSERT(to_string(std::make_pair(1, 2)), LIKE(), "pair<int,\\s?int>"_re);
        std::cout << to_string(std::make_pair(1, 2));
    };
    TEST("nullptr") {
        ASSERT(to_string(nullptr), EQ(), std::string("0"));
        ASSERT(to_string(NULL), EQ(), std::string("0"));
    };
    TEST("string_cstring") {
        std::string str("str\ning");
        ASSERT_EQ(to_string(str), "\"str\\ning\"");
        ASSERT_EQ(to_string("cstr\ring"), "\"cstr\\ring\"");
        char const* cstr = "\"cstring\"";
        ASSERT_EQ(to_string(cstr), "\"\\\"cstring\\\"\"");
    };
    TEST("char") {
        ASSERT_EQ(to_string('a'), "'a'");
        ASSERT_EQ(to_string('\n'), "'\\n'");
    };
    TEST("floating_point") {
        ASSERT(std::string("1.123"), IN(), to_string(1.123F));
        ASSERT(std::string("1.123"), IN(), to_string(1.123));
        std::cout << to_string(1.234);
    };
    TEST("not_streamable") {
        ASSERT(to_string(not_streamable()), LIKE(), "not_streamable"_re);
        std::cout << to_string(not_streamable());
    };
    TEST("streamable") {
        ASSERT(to_string(1), EQ(), std::string("1"));
    };
    TEST("derived") {
        ASSERT(to_string(derived()), LIKE(), "derived"_re);
        std::cout << to_string(derived());
    };
};

SUITE_PAR("test_traits") {
    TEST("is_streamable") {
        ASSERT_NOTHROW((throw_if_not_streamable<std::ostringstream, streamable>()));
        ASSERT_THROWS((throw_if_not_streamable<std::ostringstream, void_type>()), std::logic_error);
        ASSERT_THROWS((throw_if_not_streamable<std::ostringstream, not_streamable>()), std::logic_error);
    };
    TEST("is_iterable") {
        ASSERT_NOTHROW((throw_if_not_iterable<iterable>()));
        ASSERT_THROWS((throw_if_not_iterable<void_type>()), std::logic_error);
        ASSERT_THROWS((throw_if_not_iterable<not_iterable>()), std::logic_error);
    };
};

SUITE_PAR("test_assertions") {
    class maybe_throwing
    {
    public:
        explicit maybe_throwing(bool t_) {
            if (t_) {
                throw std::logic_error("maybe_throwing");
            }
        }
    };

    class not_copyable
    {
    public:
        not_copyable()                        = default;
        ~not_copyable()                       = default;
        not_copyable(not_copyable const&)     = delete;
        not_copyable(not_copyable&&) noexcept = default;
        auto
        operator=(not_copyable const&) -> not_copyable& = delete;
        auto
        operator=(not_copyable&&) noexcept -> not_copyable& = default;
    };

    TEST("negation") {
        // successful
        ASSERT_NOTHROW(ASSERT(1, !EQUALS(), 2));
        ASSERT_NOTHROW(ASSERT(true, !EQUALS(), false));
        ASSERT_NOTHROW(ASSERT(1.5, !LESS(), 0.3));
        ASSERT_NOTHROW(ASSERT(1, !!EQ(), 1));
        // failing
        ASSERT_THROWS(ASSERT(1, !EQ(), 1), assertion_failure);
        ASSERT_THROWS(ASSERT(2, !EQUALS(), 2), assertion_failure);
        ASSERT_THROWS(ASSERT(false, !EQUALS(), false), assertion_failure);
        ASSERT_THROWS(ASSERT(1002.4, !LESS(), 1002.5), assertion_failure);
        ASSERT_THROWS(ASSERT("hello", !EQ(), "hello"), assertion_failure);
    }
    TEST("assert") {  // successful
        ASSERT_NOTHROW(ASSERT(1, EQUALS(), 1));
        ASSERT_NOTHROW(ASSERT(true, EQUALS(), true));
        ASSERT_NOTHROW(ASSERT(1.5, LESS(), 100.3));
        ASSERT_NOTHROW(ASSERT(2, IN(), (std::vector<int>{1, 3, 2})));
        // failing
        ASSERT_THROWS(ASSERT(2, EQUALS(), 1), assertion_failure);
        ASSERT_THROWS(ASSERT(false, EQUALS(), true), assertion_failure);
        ASSERT_THROWS(ASSERT(1002.5, LESS(), 100.3), assertion_failure);
        ASSERT_THROWS(ASSERT("hello", EQ(), "world"), assertion_failure);
        ASSERT_THROWS(ASSERT(2, IN(), (std::vector<int>{1, 3})), assertion_failure);
    };
    TEST("assert_equals") {
        // successful
        ASSERT_NOTHROW(ASSERT_EQ(1, 1));
        ASSERT_NOTHROW(ASSERT_EQ(true, true));
        ASSERT_NOTHROW(ASSERT_EQ("", ""));
        ASSERT_NOTHROW(ASSERT(1.12, EQ(0.1), 1.15));
        // failing
        ASSERT_THROWS(ASSERT_EQ(1, 2), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ(false, true), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ("b", "a"), assertion_failure);
        ASSERT_THROWS(ASSERT(1.11, EQ(0.001), 1.10), assertion_failure);
    };
    TEST("assert_true") {
        // successful
        ASSERT_NOTHROW(ASSERT_TRUE(true));
        ASSERT_NOTHROW(ASSERT_TRUE(1 == 1));
        // failing
        ASSERT_THROWS(ASSERT_TRUE(false), assertion_failure);
        ASSERT_THROWS(ASSERT_TRUE(1 == 2), assertion_failure);
    };
    TEST("assert_false") {
        // successful
        ASSERT_NOTHROW(ASSERT_FALSE(false));
        ASSERT_NOTHROW(ASSERT_FALSE(1 == 2));
        // failing
        ASSERT_THROWS(ASSERT_FALSE(true), assertion_failure);
        ASSERT_THROWS(ASSERT_FALSE(1 == 1), assertion_failure);
    };
    TEST("assert_not_null") {
        // successful
        int         i = 1;
        double      d = 1.0;
        char const* s = "";
        ASSERT_NOTHROW(ASSERT_NOT_NULL(&i));
        ASSERT_NOTHROW(ASSERT_NOT_NULL(&d));
        ASSERT_NOTHROW(ASSERT_NOT_NULL(&s));
        // failing
        int* n = nullptr;
        ASSERT_THROWS(ASSERT_NOT_NULL(n), assertion_failure);
        n = NULL;
        ASSERT_THROWS(ASSERT_NOT_NULL(n), assertion_failure);
    };
    TEST("assert_null") {
        // successful
        int* n = nullptr;
        ASSERT_NOTHROW(ASSERT_NULL(n));
        n = NULL;
        ASSERT_NOTHROW(ASSERT_NULL(n));
        // failing
        int         i = 1;
        double      d = 1.0;
        char const* s = "";
        ASSERT_THROWS(ASSERT_NULL(&i), assertion_failure);
        ASSERT_THROWS(ASSERT_NULL(&d), assertion_failure);
        ASSERT_THROWS(ASSERT_NULL(&s), assertion_failure);
    };
    TEST("assert_throws") {
        // successful
        ASSERT_NOTHROW(ASSERT_THROWS(throw std::logic_error(""), std::logic_error));
        ASSERT_NOTHROW(auto a = ASSERT_THROWS(return maybe_throwing(true), std::logic_error));
        ASSERT_NOTHROW(auto a = ASSERT_THROWS(return maybe_throwing(true), std::logic_error);
                       ASSERT_EQ(std::string(a.cause().what()), "maybe_throwing"));
        // failing
        ASSERT_THROWS(ASSERT_THROWS(return, std::logic_error), assertion_failure);
        ASSERT_THROWS(ASSERT_THROWS(throw std::runtime_error(""), std::logic_error), assertion_failure);
        ASSERT_THROWS(ASSERT_THROWS(throw 1, std::logic_error), assertion_failure);
        ASSERT_THROWS(auto a = ASSERT_THROWS(return maybe_throwing(false), std::logic_error), assertion_failure);
        ASSERT_THROWS(auto a = ASSERT_THROWS(return maybe_throwing(false), std::logic_error);
                      ASSERT_EQ(a.cause().what(), ""), assertion_failure);
    };
    TEST("assert_nothrow") {
        // successful
        ASSERT_NOTHROW(ASSERT_NOTHROW(return ));
        ASSERT_NOTHROW(auto a = ASSERT_NOTHROW(return 1); ASSERT_EQ(a, 1));
        ASSERT_NOTHROW(auto a = ASSERT_NOTHROW(return maybe_throwing(false)));
        ASSERT_NOTHROW(auto a = ASSERT_NOTHROW(return not_copyable()));
        not_copyable nc;
        ASSERT_NOTHROW(auto a = ASSERT_NOTHROW(return &nc));
        ASSERT_NOTHROW(auto a = ASSERT_NOTHROW(return std::ref(nc)));
        ASSERT_NOTHROW(auto a = ASSERT_NOTHROW(return std::move(nc)));
        // failing
        ASSERT_THROWS(ASSERT_NOTHROW(throw std::runtime_error("")), assertion_failure);
        ASSERT_THROWS(ASSERT_NOTHROW(throw 1), assertion_failure);
    };
    TEST("assert_runtime") {
        // successful
        ASSERT_NOTHROW(ASSERT_RUNTIME(return, 100));
        ASSERT_NOTHROW(auto a = ASSERT_RUNTIME(return 1, 100); ASSERT_EQ(a, 1));
        ASSERT_NOTHROW(auto a = ASSERT_RUNTIME(return maybe_throwing(false), 100));
        ASSERT_NOTHROW(auto a = ASSERT_RUNTIME(return not_copyable(), 100));
        not_copyable nc;
        ASSERT_NOTHROW(auto a = ASSERT_RUNTIME(return &nc, 100));
        ASSERT_NOTHROW(auto a = ASSERT_RUNTIME(return std::ref(nc), 100));
        ASSERT_NOTHROW(auto a = ASSERT_RUNTIME(return std::move(nc), 100));
        // failing
        ASSERT_THROWS(ASSERT_RUNTIME(std::this_thread::sleep_for(std::chrono::milliseconds(100)), 10),
                      assertion_failure);
        ASSERT_THROWS(auto a =
                        ASSERT_RUNTIME(std::this_thread::sleep_for(std::chrono::milliseconds(100)); return 1, 10);
                      ASSERT_EQ(a, 1), assertion_failure);
        ASSERT_THROWS(ASSERT_RUNTIME(throw std::logic_error(""), 100), std::logic_error);
    };
};

DESCRIBE("test_output_capture") {
    IT("should capture the output in a single thread") {
        auto ts = testsuite::create("ts");
        for (int i = 1; i < 9; ++i) {
            ts->test("capture", [i] {
                std::cout << 'o' << "ut from " << i;
                std::cerr << 'e' << "rr from " << i;
            });
        }
        ts->run();
        for (auto i = 0UL; i < ts->testcases().size(); ++i) {
            auto const& tc = ts->testcases().at(i);
            ASSERT_EQ(tc.cout(), std::string("out from ") + to_string(i + 1));
            ASSERT_EQ(tc.cerr(), std::string("err from ") + to_string(i + 1));
        }
    };
    IT("should capture the output in multiple threads") {
        auto ts = testsuite_parallel::create("ts");
        for (int i = 1; i < 9; ++i) {
            ts->test("capture", [i] {
                std::cout << 'o' << "ut from " << i;
                std::cerr << 'e' << "rr from " << i;
            });
        }
        ts->run();
        for (auto i = 0UL; i < ts->testcases().size(); ++i) {
            auto const& tc = ts->testcases().at(i);
            ASSERT_EQ(tc.cout(), std::string("out from ") + to_string(i + 1));
            ASSERT_EQ(tc.cerr(), std::string("err from ") + to_string(i + 1));
        }
    };
};

DESCRIBE("test_suite_meta_functions") {
    int  x                  = -3;
    int  y                  = -3;
    int  setup_called       = 0;
    int  before_each_called = 0;
    int  after_each_called  = 0;
    bool teardown_called    = false;
    SETUP() {
        ASSERT_EQ(setup_called, 0);
        ASSERT_EQ(before_each_called, 0);
        ASSERT_EQ(after_each_called, 0);
        ASSERT_FALSE(teardown_called);
        x = 0;
        y = 0;
        setup_called += 1;
    };
    BEFORE_EACH() {
        ASSERT_EQ(setup_called, 1);
        ASSERT_FALSE(teardown_called);
        y += 1;
        before_each_called += 1;
    };
    AFTER_EACH() {
        ASSERT_EQ(setup_called, 1);
        ASSERT_FALSE(teardown_called);
        y -= 1;
        after_each_called += 1;
    };
    TEARDOWN() {
        teardown_called = true;
        ASSERT_EQ(setup_called, 1);
        ASSERT_EQ(before_each_called, 3);
        ASSERT_EQ(after_each_called, before_each_called);
    };
    IT("should setup x,y with 0") {
        ASSERT_EQ(before_each_called, 1);
        ASSERT_EQ(after_each_called, 0);
        ASSERT_EQ(x, 0);
        ASSERT_EQ(y - 1, 0);
    };
    IT("should increment y before") {
        ASSERT_EQ(before_each_called, 2);
        ASSERT_EQ(after_each_called, 1);
        ASSERT_EQ(y, 1);
    };
    IT("should decrement y after") {
        ASSERT_EQ(before_each_called, 3);
        ASSERT_EQ(after_each_called, 2);
        ASSERT_EQ(y, 1);
    };
};

#ifdef TPP_INTERN_SYS_UNIX
#    pragma GCC diagnostic pop
#endif
