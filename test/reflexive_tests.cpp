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

#ifdef _OPENMP
#    include <omp.h>
#else
#    define omp_get_max_threads() 1
#endif

#ifdef TPP_INTERN_SYS_UNIX
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wunused-variable"
#    pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

using tpp::operator""_re;
using tpp::operator""_re_i;
using tpp::reporter_ptr;
using tpp::intern::to_string;
using tpp::intern::assert::assertion_failure;
using tpp::intern::report::console_reporter;
using tpp::intern::report::json_reporter;
using tpp::intern::report::markdown_reporter;
using tpp::intern::report::reporter_config;
using tpp::intern::report::reporter_factory;
using tpp::intern::report::xml_reporter;
using tpp::intern::test::statistic;
using tpp::intern::test::testcase;
using tpp::intern::test::testsuite;
using tpp::intern::test::testsuite_parallel;
using tpp::intern::test::testsuite_ptr;

SUITE_PAR("test_assert") {
    TEST("equals") {
        // successful assertion
        ASSERT_NOTHROW(ASSERT_EQ('a', 'a'));
        ASSERT_NOTHROW(ASSERT_EQ(true, true));
        ASSERT_NOTHROW(ASSERT_EQ("a", "a"));
        ASSERT_NOTHROW(ASSERT_EQ(std::string("a"), std::string("a")));
        ASSERT_NOTHROW(ASSERT_EQ(std::string("a"), "a"));
        ASSERT_NOTHROW(ASSERT_EQ((std::vector<int>{1, 2}), (std::vector<int>{1, 2})));
        ASSERT_NOTHROW(ASSERT_EQ(1, 1));
        ASSERT_NOTHROW(ASSERT_EQ(1., 1.));
        ASSERT_NOTHROW(ASSERT_EQ(1.F, 1.F));
        ASSERT_NOTHROW(ASSERT(1, EQ, 1));
        // successful negated assertion
        ASSERT_NOTHROW(ASSERT_NOT_EQ('a', 'b'));
        ASSERT_NOTHROW(ASSERT_NOT_EQ(true, false));
        ASSERT_NOTHROW(ASSERT_NOT_EQ("a", "b"));
        ASSERT_NOTHROW(ASSERT_NOT_EQ(std::string("a"), std::string("b")));
        ASSERT_NOTHROW(ASSERT_NOT_EQ(std::string("a"), "b"));
        ASSERT_NOTHROW(ASSERT_NOT_EQ((std::vector<int>{1, 3}), (std::vector<int>{1, 2})));
        ASSERT_NOTHROW(ASSERT_NOT_EQ(1, 2));
        ASSERT_NOTHROW(ASSERT_NOT_EQ(1., 2.));
        ASSERT_NOTHROW(ASSERT_NOT_EQ(1.F, 2.F));
        ASSERT_NOTHROW(ASSERT_NOT(1, EQ, 2));
        // failed assertion
        ASSERT_THROWS(ASSERT_EQ('a', 'b'), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ(true, false), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ("a", "b"), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ(std::string("a"), std::string("b")), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ(std::string("a"), "b"), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ((std::vector<int>{1, 2}), (std::vector<int>{2, 3})), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ(1, 2), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ(1., 2.), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ(1.F, 2.F), assertion_failure);
        ASSERT_THROWS(ASSERT(1, EQ, 2), assertion_failure);
        // failed negated assertion
        ASSERT_THROWS(ASSERT_NOT_EQ('a', 'a'), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_EQ(true, true), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_EQ("a", "a"), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_EQ(std::string("a"), std::string("a")), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_EQ(std::string("a"), "a"), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_EQ((std::vector<int>{1, 2}), (std::vector<int>{1, 2})), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_EQ(1, 1), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_EQ(1., 1.), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_EQ(1.F, 1.F), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT(1, EQ, 1), assertion_failure);

        auto f = ASSERT_THROWS(ASSERT_EQ(1, 2), assertion_failure);
        ASSERT_LIKE(f.what(), "Expected 1 to be equals 2"_re);
    };
    TEST("equals float") {
        // successful assertion
        ASSERT_NOTHROW(ASSERT_EQ(1., 1.));
        ASSERT_NOTHROW(ASSERT_EQ(1.0011, 1.0012, .001));
        ASSERT_NOTHROW(ASSERT_EQ(1.F, 1.F));
        ASSERT_NOTHROW(ASSERT(1., EQ, 1.));
        ASSERT_NOTHROW(ASSERT(1.001, EQ, 1.001, .001));
        // failed assertion
        ASSERT_THROWS(ASSERT_EQ(1., 2.), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ(1.0011, 2.0012, .0001), assertion_failure);
        ASSERT_THROWS(ASSERT_EQ(1.F, 2.F), assertion_failure);
        ASSERT_THROWS(ASSERT(1., EQ, 2.), assertion_failure);
        ASSERT_THROWS(ASSERT(1.0011, EQ, 2.0012, .0001), assertion_failure);

        auto f = ASSERT_THROWS(ASSERT_EQ(1.1, 2.1), assertion_failure);
        ASSERT_LIKE(f.what(), "Expected 1.1\\d* to be equals 2.1\\d*"_re);
    };
    TEST("greater") {
        // successful assertion
        ASSERT_NOTHROW(ASSERT_GT(2, 1));
        ASSERT_NOTHROW(ASSERT_GT('b', 'a'));
        ASSERT_NOTHROW(ASSERT_GT(2., 1.));
        ASSERT_NOTHROW(ASSERT(2, GT, 1));
        // successful negated assertion
        ASSERT_NOTHROW(ASSERT_NOT_GT(1, 2));
        ASSERT_NOTHROW(ASSERT_NOT_GT('a', 'b'));
        ASSERT_NOTHROW(ASSERT_NOT_GT(1., 2.));
        ASSERT_NOTHROW(ASSERT_NOT(2, GT, 2));
        // failed assertion
        ASSERT_THROWS(ASSERT_GT(1, 2), assertion_failure);
        ASSERT_THROWS(ASSERT_GT('a', 'b'), assertion_failure);
        ASSERT_THROWS(ASSERT_GT(1., 2.), assertion_failure);
        ASSERT_THROWS(ASSERT(2, GT, 2), assertion_failure);
        // failed negated assertion
        ASSERT_THROWS(ASSERT_NOT_GT(2, 1), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_GT('b', 'a'), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_GT(2., 1.), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT(2, GT, 1), assertion_failure);

        auto f = ASSERT_THROWS(ASSERT(2, GT, 2), assertion_failure);
        ASSERT_LIKE(f.what(), "Expected 2 to be greater than 2"_re);
    };
    TEST("less") {
        // successful assertion
        ASSERT_NOTHROW(ASSERT_LT(1, 2));
        ASSERT_NOTHROW(ASSERT_LT('a', 'b'));
        ASSERT_NOTHROW(ASSERT_LT(1., 2.));
        ASSERT_NOTHROW(ASSERT(1, LT, 2));
        // successful negated assertion
        ASSERT_NOTHROW(ASSERT_NOT_LT(2, 1));
        ASSERT_NOTHROW(ASSERT_NOT_LT('b', 'a'));
        ASSERT_NOTHROW(ASSERT_NOT_LT(2., 1.));
        ASSERT_NOTHROW(ASSERT_NOT(2, LT, 2));
        // failed assertion
        ASSERT_THROWS(ASSERT_LT(2, 1), assertion_failure);
        ASSERT_THROWS(ASSERT_LT('b', 'a'), assertion_failure);
        ASSERT_THROWS(ASSERT_LT(2., 1.), assertion_failure);
        ASSERT_THROWS(ASSERT(2, LT, 2), assertion_failure);
        // failed negated assertion
        ASSERT_THROWS(ASSERT_NOT_LT(1, 2), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_LT('a', 'b'), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_LT(1., 2.), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT(1, LT, 2), assertion_failure);

        auto f = ASSERT_THROWS(ASSERT(2, LT, 2), assertion_failure);
        ASSERT_LIKE(f.what(), "Expected 2 to be less than 2"_re);
    };
    TEST("in") {
        // successful assertion
        ASSERT_NOTHROW(ASSERT_IN(1, std::vector<int>{1}));
        ASSERT_NOTHROW(ASSERT_IN('a', std::vector<int>{'a'}));
        ASSERT_NOTHROW(ASSERT_IN("a", std::string("a")));
        ASSERT_NOTHROW(ASSERT_IN('a', std::string("a")));
        ASSERT_NOTHROW(ASSERT(1, IN, std::vector<int>{1}));
        // successful negated assertion
        ASSERT_NOTHROW(ASSERT_NOT_IN(2, std::vector<int>{1}));
        ASSERT_NOTHROW(ASSERT_NOT_IN('b', std::vector<int>{'a'}));
        ASSERT_NOTHROW(ASSERT_NOT_IN("b", std::string("a")));
        ASSERT_NOTHROW(ASSERT_NOT_IN('b', std::string("a")));
        ASSERT_NOTHROW(ASSERT_NOT(2, IN, std::vector<int>{1}));
        // failed assertion
        ASSERT_THROWS(ASSERT_IN(2, std::vector<int>{1}), assertion_failure);
        ASSERT_THROWS(ASSERT_IN('b', std::vector<int>{'a'}), assertion_failure);
        ASSERT_THROWS(ASSERT_IN("b", std::string("a")), assertion_failure);
        ASSERT_THROWS(ASSERT_IN('b', std::string("a")), assertion_failure);
        ASSERT_THROWS(ASSERT(2, IN, std::vector<int>{1}), assertion_failure);
        // failed negated assertion
        ASSERT_THROWS(ASSERT_NOT_IN(1, std::vector<int>{1}), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_IN('a', std::vector<int>{'a'}), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_IN("a", std::string("a")), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_IN('a', std::string("a")), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT(1, IN, std::vector<int>{1}), assertion_failure);

        auto f = ASSERT_THROWS(ASSERT(2, IN, std::vector<int>{1}), assertion_failure);
        ASSERT_LIKE(f.what(), "Expected 2 to be in .*vector"_re);
    };
    TEST("match") {
        std::cmatch cm;
        std::smatch sm;
        // successful assertion
        ASSERT_NOTHROW(ASSERT_MATCH("hello world", ".*"_re));
        ASSERT_NOTHROW(ASSERT_MATCH("hello world", "HELLO WORLD"_re_i));
        ASSERT_NOTHROW(ASSERT_MATCH("hello world", ".*"));
        ASSERT_NOTHROW(ASSERT_MATCH("hello world 11", "\\S{5}\\s.*?\\d+"_re));
        ASSERT_NOTHROW(ASSERT_MATCH(std::string("hello world"), ".*"_re));
        ASSERT_NOTHROW(ASSERT("hello world", MATCH, ".*"_re));
        // successful negated assertion
        ASSERT_NOTHROW(ASSERT_NOT_MATCH("hello world", "\\s*"_re));
        ASSERT_NOTHROW(ASSERT_NOT_MATCH("hello world", "AAA"_re_i));
        ASSERT_NOTHROW(ASSERT_NOT_MATCH("hello world", "fff"));
        ASSERT_NOTHROW(ASSERT_NOT_MATCH("hello world 11", "\\S{7}\\s.*?\\d"_re));
        ASSERT_NOTHROW(ASSERT_NOT_MATCH(std::string("hello world"), "[+-]\\d+"_re));
        ASSERT_NOTHROW(ASSERT_NOT("hello world", MATCH, "\\s*"_re));
        // failed assertion
        ASSERT_THROWS(ASSERT_MATCH("hello world", "\\s*"_re), assertion_failure);
        ASSERT_THROWS(ASSERT_MATCH("hello world", "AAA"_re_i), assertion_failure);
        ASSERT_THROWS(ASSERT_MATCH("hello world", "fff", cm), assertion_failure);
        ASSERT_THROWS(ASSERT_MATCH("hello world 11", "\\S{7}\\s.*?\\d"_re), assertion_failure);
        ASSERT_THROWS(ASSERT_MATCH(std::string("hello world"), "[+-]\\d+"_re), assertion_failure);
        ASSERT_THROWS(ASSERT("hello world", MATCH, "\\s*"_re), assertion_failure);
        // failed negated assertion
        ASSERT_THROWS(ASSERT_NOT_MATCH("hello world", ".*"_re), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_MATCH("hello world", "HELLO WORLD"_re_i), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_MATCH("hello world", ".*"), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_MATCH("hello world 11", "\\S{5}\\s.*?\\d+"_re), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_MATCH(std::string("hello world"), ".*"_re), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT("hello world", MATCH, ".*"_re), assertion_failure);

        std::string s{"hello world 11"};
        ASSERT_NOTHROW(ASSERT("hello world 11", MATCH, "(\\S{5})\\s(.*?)(\\d+)"_re, cm));
        ASSERT_EQ(cm.str(1), "hello");
        ASSERT_EQ(cm.str(2), "world ");
        ASSERT_EQ(cm.str(3), "11");
        ASSERT_NOTHROW(ASSERT_MATCH(s, "(\\S{5})\\s(.*?)(\\d+)"_re, sm));
        ASSERT_EQ(sm.str(1), "hello");
        ASSERT_EQ(sm.str(2), "world ");
        ASSERT_EQ(sm.str(3), "11");

        auto f = ASSERT_THROWS(ASSERT_MATCH("hello world", "\\s*"_re), assertion_failure);
        ASSERT_LIKE(f.what(), "Expected .* to match .*"_re);
    };
    TEST("like") {
        std::cmatch cm;
        std::smatch sm;
        // successful assertion
        ASSERT_NOTHROW(ASSERT_LIKE("hello world", "hell"_re));
        ASSERT_NOTHROW(ASSERT_LIKE("hello world", "HELL"_re_i));
        ASSERT_NOTHROW(ASSERT_LIKE("hello world", ".*?"));
        ASSERT_NOTHROW(ASSERT_LIKE("hello world 11", "\\S{5}"_re));
        ASSERT_NOTHROW(ASSERT_LIKE(std::string("hello world"), "hell"_re));
        ASSERT_NOTHROW(ASSERT("hello world", LIKE, "hell"_re));
        // successful negated assertion
        ASSERT_NOTHROW(ASSERT_NOT_LIKE("hello world", "blub"_re));
        ASSERT_NOTHROW(ASSERT_NOT_LIKE("hello world", "AAA"_re_i));
        ASSERT_NOTHROW(ASSERT_NOT_LIKE("hello world", "AAA"));
        ASSERT_NOTHROW(ASSERT_NOT_LIKE("hello world 11", "\\S{7}"_re));
        ASSERT_NOTHROW(ASSERT_NOT_LIKE(std::string("hello world"), "[+-]\\d+"_re));
        ASSERT_NOTHROW(ASSERT_NOT("hello world", LIKE, "blub"_re));
        // failed assertion
        ASSERT_THROWS(ASSERT_LIKE("hello world", "blub"_re), assertion_failure);
        ASSERT_THROWS(ASSERT_LIKE("hello world", "AAA"_re_i), assertion_failure);
        ASSERT_THROWS(ASSERT_LIKE("hello world", "AAA", cm), assertion_failure);
        ASSERT_THROWS(ASSERT_LIKE("hello world 11", "\\S{7}"_re), assertion_failure);
        ASSERT_THROWS(ASSERT_LIKE(std::string("hello world"), "[+-]\\d+"_re), assertion_failure);
        ASSERT_THROWS(ASSERT("hello world", LIKE, "blub"_re), assertion_failure);
        // failed negated assertion
        ASSERT_THROWS(ASSERT_NOT_LIKE("hello world", "hell"_re), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_LIKE("hello world", "HELL"_re_i), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_LIKE("hello world", ".*?"), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_LIKE("hello world 11", "\\S{5}"_re), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT_LIKE(std::string("hello world"), "hell"_re), assertion_failure);
        ASSERT_THROWS(ASSERT_NOT("hello world", LIKE, "hell"_re), assertion_failure);

        std::string s{"hello world 11"};
        ASSERT_NOTHROW(ASSERT("hello world 11", LIKE, ".*?(\\d+)"_re, cm));
        ASSERT_EQ(cm.str(1), "11");
        ASSERT_NOTHROW(ASSERT_LIKE(s, ".*?(\\d+)"_re, sm));
        ASSERT_EQ(sm.str(1), "11");

        auto f = ASSERT_THROWS(ASSERT_LIKE("hello world", "\\s{4}"_re), assertion_failure);
        ASSERT_LIKE(f.what(), "Expected .* to be like .*"_re);
    };
};

SUITE_PAR("test_assert_functional") {
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

    TEST("assert_true") {
        // successful assertion
        ASSERT_NOTHROW(ASSERT_TRUE(true));
        ASSERT_NOTHROW(ASSERT_TRUE(1 == 1));
        // failed assertion
        ASSERT_THROWS(ASSERT_TRUE(false), assertion_failure);
        ASSERT_THROWS(ASSERT_TRUE(1 == 2), assertion_failure);

        auto f = ASSERT_THROWS(ASSERT_TRUE(false), assertion_failure);
        ASSERT_LIKE(f.what(), "Expected false to be equals true"_re);
    };
    TEST("assert_false") {
        // successful assertion
        ASSERT_NOTHROW(ASSERT_FALSE(false));
        ASSERT_NOTHROW(ASSERT_FALSE(1 == 2));
        // failed assertion
        ASSERT_THROWS(ASSERT_FALSE(true), assertion_failure);
        ASSERT_THROWS(ASSERT_FALSE(1 == 1), assertion_failure);

        auto f = ASSERT_THROWS(ASSERT_FALSE(true), assertion_failure);
        ASSERT_LIKE(f.what(), "Expected true to be equals false"_re);
    };
    TEST("assert_not_null") {
        // successful assertion
        int         i = 1;
        double      d = 1.0;
        char const* s = "";
        ASSERT_NOTHROW(ASSERT_NOT_NULL(&i));
        ASSERT_NOTHROW(ASSERT_NOT_NULL(&d));
        ASSERT_NOTHROW(ASSERT_NOT_NULL(&s));
        // failed assertion
        int* n = nullptr;
        ASSERT_THROWS(ASSERT_NOT_NULL(n), assertion_failure);
        n = NULL;
        ASSERT_THROWS(ASSERT_NOT_NULL(n), assertion_failure);

        auto f = ASSERT_THROWS(ASSERT_NOT_NULL(n), assertion_failure);
        ASSERT_LIKE(f.what(), "Expected .*? to be not equals 0"_re);
    };
    TEST("assert_null") {
        // successful assertion
        int* n = nullptr;
        ASSERT_NOTHROW(ASSERT_NULL(n));
        n = NULL;
        ASSERT_NOTHROW(ASSERT_NULL(n));
        // failed assertion
        int         i = 1;
        double      d = 1.0;
        char const* s = "";
        ASSERT_THROWS(ASSERT_NULL(&i), assertion_failure);
        ASSERT_THROWS(ASSERT_NULL(&d), assertion_failure);
        ASSERT_THROWS(ASSERT_NULL(&s), assertion_failure);

        auto f = ASSERT_THROWS(ASSERT_NULL(&i), assertion_failure);
        ASSERT_LIKE(f.what(), "Expected .*? to be equals 0"_re);
    };
    TEST("assert_throws") {
        // successful assertion
        ASSERT_NOTHROW(ASSERT_THROWS(throw std::logic_error(""), std::logic_error));
        ASSERT_NOTHROW(auto a = ASSERT_THROWS(return maybe_throwing(true), std::logic_error));
        ASSERT_NOTHROW(auto a = ASSERT_THROWS(return maybe_throwing(true), std::logic_error);
                       ASSERT_EQ(std::string(a.what()), "maybe_throwing"));
        // failed assertion
        ASSERT_THROWS(ASSERT_THROWS(return, std::logic_error), assertion_failure);
        ASSERT_THROWS(ASSERT_THROWS(throw std::runtime_error(""), std::logic_error), assertion_failure);
        ASSERT_THROWS(ASSERT_THROWS(throw 1, std::logic_error), assertion_failure);
        ASSERT_THROWS(auto a = ASSERT_THROWS(return maybe_throwing(false), std::logic_error), assertion_failure);
        ASSERT_THROWS(auto a = ASSERT_THROWS(return maybe_throwing(false), std::logic_error);
                      ASSERT_EQ(a.what(), ""), assertion_failure);

        auto f = ASSERT_THROWS(ASSERT_THROWS(return, std::logic_error), assertion_failure);
        ASSERT_LIKE(f.what(), "No exception thrown, expected .*?logic_error"_re);
    };
    TEST("assert_nothrow") {
        // successful assertion
        ASSERT_NOTHROW(ASSERT_NOTHROW(return ));
        ASSERT_NOTHROW(auto a = ASSERT_NOTHROW(return 1); ASSERT_EQ(a, 1));
        ASSERT_NOTHROW(auto a = ASSERT_NOTHROW(return maybe_throwing(false)));
        ASSERT_NOTHROW(auto a = ASSERT_NOTHROW(return not_copyable()));
        not_copyable nc;
        ASSERT_NOTHROW(auto a = ASSERT_NOTHROW(return &nc));
        ASSERT_NOTHROW(auto a = ASSERT_NOTHROW(return std::ref(nc)));
        ASSERT_NOTHROW(auto a = ASSERT_NOTHROW(return std::move(nc)));
        // failed assertion
        ASSERT_THROWS(ASSERT_NOTHROW(throw std::runtime_error("")), assertion_failure);
        ASSERT_THROWS(ASSERT_NOTHROW(throw 1), assertion_failure);

        auto f = ASSERT_THROWS(ASSERT_NOTHROW(throw std::runtime_error("")), assertion_failure);
        ASSERT_LIKE(f.what(), "Expected no exception, caught .*?runtime_error"_re);
    };
    TEST("assert_runtime") {
        // successful assertion
        ASSERT_NOTHROW(ASSERT_RUNTIME(return, 100));
        ASSERT_NOTHROW(auto a = ASSERT_RUNTIME(return 1, 100); ASSERT_EQ(a, 1));
        ASSERT_NOTHROW(auto a = ASSERT_RUNTIME(return maybe_throwing(false), 100));
        ASSERT_NOTHROW(auto a = ASSERT_RUNTIME(return not_copyable(), 100));
        not_copyable nc;
        ASSERT_NOTHROW(auto a = ASSERT_RUNTIME(return &nc, 100));
        ASSERT_NOTHROW(auto a = ASSERT_RUNTIME(return std::ref(nc), 100));
        ASSERT_NOTHROW(auto a = ASSERT_RUNTIME(return std::move(nc), 100));
        // failed assertion
        ASSERT_THROWS(ASSERT_RUNTIME(std::this_thread::sleep_for(std::chrono::milliseconds(100)), 10),
                      assertion_failure);
        ASSERT_THROWS(auto a =
                        ASSERT_RUNTIME(std::this_thread::sleep_for(std::chrono::milliseconds(100)); return 1, 10);
                      ASSERT_EQ(a, 1), assertion_failure);
        ASSERT_THROWS(ASSERT_RUNTIME(throw std::logic_error(""), 100), std::logic_error);

        auto f = ASSERT_THROWS(ASSERT_RUNTIME(std::this_thread::sleep_for(std::chrono::milliseconds(100)), 10),
                               assertion_failure);
        ASSERT_LIKE(f.what(), "Expected the runtime to be less 10"_re);
    };
};

SUITE("test_testsuite_parallel") {
    TEST("parallel_run") {
        testsuite_ptr ts = testsuite_parallel::create("ts");
        ts->test("", [] { std::this_thread::sleep_for(std::chrono::milliseconds(1000)); });
        ts->test("", [] { std::this_thread::sleep_for(std::chrono::milliseconds(1000)); });
        ts->test("", [] { ASSERT_TRUE(false); });
        ts->test("", [] { throw std::logic_error(""); });
        std::cout << "max threads: " << omp_get_max_threads() << std::flush;
        ASSERT_RUNTIME(ts->run(), 2500);
        if (omp_get_max_threads() == 1) {
            ASSERT(ts->statistics().elapsed_time(), GT, 2000);
            double t = 0.0;
            for (auto const& tc : ts->testcases()) {
                t += tc.elapsed_time();
            }
            ASSERT_GT(ts->statistics().elapsed_time(), t);
        } else {
            ASSERT(ts->statistics().elapsed_time(), LT, 2000);
        }
        statistic const& stat = ts->statistics();
        ASSERT_EQ(stat.tests(), 4UL);
        ASSERT_EQ(stat.errors(), 1UL);
        ASSERT_EQ(stat.failures(), 1UL);
        ASSERT_EQ(stat.successes(), 2UL);
    };
};

SUITE("test_testsuite") {
    TEST("creation") {
        auto a = std::chrono::system_clock::now();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        testsuite_ptr ts = testsuite::create("ts");
        ASSERT(ts->timestamp(), GT, a);
        ASSERT(ts->name(), EQ, std::string("ts"));
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
        ASSERT(tc1.suite_name(), EQ, std::string("ts"));
        ASSERT(tc2.suite_name(), EQ, std::string("ts"));
        ASSERT(tc3.suite_name(), EQ, std::string("ts"));
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
        ASSERT_LT(t, ts->statistics().elapsed_time());
    };
};

SUITE_PAR("test_testcase") {
    TEST("creation") {
        testcase tc({"t1", "ctx"}, [] {});
        testcase tc2({"t2", ""}, [] {});
        ASSERT_EQ(tc.result(), testcase::IS_UNDONE);
        ASSERT(tc.suite_name(), EQ, std::string("ctx"));
        ASSERT(tc2.suite_name(), EQ, std::string(""));
        ASSERT(tc.name(), EQ, std::string("t1"));
    };
    TEST("successful_execution") {
        testcase tc({"t1", "ctx"}, [] {});
        tc();
        ASSERT_EQ(tc.result(), testcase::HAS_PASSED);
        ASSERT(tc.elapsed_time(), GT, 0.0);
        ASSERT_EQ(tc.reason().size(), 0UL);
    };
    TEST("failed_execution") {
        testcase tc({"t1", "ctx"}, [] { ASSERT_TRUE(false); });
        tc();
        ASSERT_EQ(tc.result(), testcase::HAS_FAILED);
        ASSERT(tc.elapsed_time(), GT, 0.0);
    };
    TEST("erroneous_execution") {
        testcase tc({"t1", "ctx"}, [] { throw std::logic_error("err"); });
        tc();
        ASSERT_EQ(tc.result(), testcase::HAD_ERROR);
        ASSERT(tc.elapsed_time(), GT, 0.0);
        ASSERT(tc.reason(), EQ, std::string("err"));

        testcase tc2({"t2", "ctx"}, [] { throw 1; });
        tc2();
        ASSERT_EQ(tc2.result(), testcase::HAD_ERROR);
        ASSERT(tc2.elapsed_time(), GT, 0.0);
        ASSERT(tc2.reason(), EQ, std::string("unknown error"));
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
        ASSERT(to_string(true), EQ, std::string("true"));
        ASSERT(to_string(false), EQ, std::string("false"));
    };
    TEST("std_pair") {
        ASSERT(to_string(std::make_pair(1, 2)), LIKE, "pair<int,\\s?int>"_re);
        std::cout << to_string(std::make_pair(1, 2));
    };
    TEST("nullptr") {
        ASSERT(to_string(nullptr), EQ, std::string("0"));
        ASSERT(to_string(NULL), EQ, std::string("0"));
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
        ASSERT(std::string("1.123"), IN, to_string(1.123F));
        ASSERT(std::string("1.123"), IN, to_string(1.123));
        std::cout << to_string(1.234);
    };
    TEST("not_streamable") {
        ASSERT(to_string(not_streamable()), LIKE, "not_streamable"_re);
        std::cout << to_string(not_streamable());
    };
    TEST("streamable") {
        ASSERT(to_string(1), EQ, std::string("1"));
    };
    TEST("derived") {
        ASSERT(to_string(derived()), LIKE, "derived"_re);
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

SUITE("test_reporters") {
    testsuite_ptr     t_ts;
    reporter_config   t_cfg;
    std::stringstream t_ss;

    SETUP() {
        t_ts = testsuite::create("testsuite");
        t_ts->test("test1", [] { ASSERT_TRUE(true); });
        t_ts->test("test2", [] {
            std::cout << "hello" << std::flush;
            ASSERT_TRUE(false);
        });
        t_ts->test("test3", [] { throw std::logic_error("error"); });
        t_ts->run();
        t_cfg.ostream = &t_ss;
    }

    BEFORE_EACH() {
        t_cfg.strip       = false;
        t_cfg.capture_out = true;
    }

    AFTER_EACH() {
        t_ss.str("");
        t_ss.clear();
    }

    TEST("console_reporter") {
        auto uut = reporter_factory::make<console_reporter>(t_cfg);
        uut->begin_report();
        uut->report(t_ts);
        uut->end_report();
        std::smatch m;
        std::string line;
        auto const  suite_re{R"(--- (.*?) \(\d\.\d+ms\) ---)"_re};
        auto const  case_re{R"( (.*?) \(\d\.\d+ms\))"_re};
        auto const  out_re{"  std(out|err) = \"(.*?)\""_re};
        auto const  res_re{"  (.*?)"_re};
        // testsuite
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, suite_re, m);
        ASSERT_EQ(m.str(1), "testsuite");
        // test1
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, case_re, m);
        ASSERT_EQ(m.str(1), "test1");

        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, out_re, m);
        ASSERT_EQ(m.str(1), "out");
        ASSERT_EQ(m.str(2), "");

        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, out_re, m);
        ASSERT_EQ(m.str(1), "err");
        ASSERT_EQ(m.str(2), "");

        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, res_re, m);
        ASSERT_EQ(m.str(1), "PASSED!");
        // test2
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, case_re, m);
        ASSERT_EQ(m.str(1), "test2");

        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, out_re, m);
        ASSERT_EQ(m.str(1), "out");
        ASSERT_EQ(m.str(2), "hello");

        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, out_re, m);
        ASSERT_EQ(m.str(1), "err");
        ASSERT_EQ(m.str(2), "");

        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, res_re, m);
        ASSERT_LIKE(m.str(1), "FAILED! Expected false to be equals true at.*");
        // test3
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, case_re, m);
        ASSERT_EQ(m.str(1), "test3");

        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, out_re, m);
        ASSERT_EQ(m.str(1), "out");
        ASSERT_EQ(m.str(2), "");

        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, out_re, m);
        ASSERT_EQ(m.str(1), "err");
        ASSERT_EQ(m.str(2), "");

        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, res_re, m);
        ASSERT_EQ(m.str(1), "ERROR! error");
        // result
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, "=== Result ==="_re);
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, R"(passes: (\d+/\d+) failures: (\d+/\d+) errors: (\d+/\d+) \(\d\.\d+ms\))"_re, m);
        ASSERT_EQ(m.str(1), "1/3");
        ASSERT_EQ(m.str(2), "1/3");
        ASSERT_EQ(m.str(3), "1/3");
    };
    TEST("json_reporter") {
        std::smatch m;
        std::string line;
        auto const  prop_re{"\\s*?\"(.*?)\": (.*?),?"_re};

        {  // unstripped
            auto uut = reporter_factory::make<json_reporter>(t_cfg);
            uut->begin_report();
            uut->report(t_ts);
            uut->end_report();

            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "{");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "  \"testsuites\": [");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "    {");
            // testsuite
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "name");
            ASSERT_EQ(m.str(2), "\"testsuite\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "time");
            ASSERT_MATCH(m.str(2), "\\d+\\.\\d+"_re);
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "count");
            ASSERT_EQ(m.str(2), "3");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "passes");
            ASSERT_EQ(m.str(2), "1");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "failures");
            ASSERT_EQ(m.str(2), "1");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "errors");
            ASSERT_EQ(m.str(2), "1");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "      \"tests\": [");
            // test1
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "        {");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "name");
            ASSERT_EQ(m.str(2), "\"test1\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "result");
            ASSERT_EQ(m.str(2), "\"success\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "reason");
            ASSERT_EQ(m.str(2), "\"\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "time");
            ASSERT_MATCH(m.str(2), "\\d+\\.\\d+"_re);
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "stdout");
            ASSERT_EQ(m.str(2), "\"\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "stderr");
            ASSERT_EQ(m.str(2), "\"\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "        },");
            // test2
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "        {");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "name");
            ASSERT_EQ(m.str(2), "\"test2\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "result");
            ASSERT_EQ(m.str(2), "\"failure\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "reason");
            ASSERT_MATCH(m.str(2), "\"Expected false to be equals true at.*\""_re);
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "time");
            ASSERT_MATCH(m.str(2), "\\d+\\.\\d+"_re);
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "stdout");
            ASSERT_EQ(m.str(2), "\"hello\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "stderr");
            ASSERT_EQ(m.str(2), "\"\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "        },");
            // test3
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "        {");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "name");
            ASSERT_EQ(m.str(2), "\"test3\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "result");
            ASSERT_EQ(m.str(2), "\"error\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "reason");
            ASSERT_EQ(m.str(2), "\"error\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "time");
            ASSERT_MATCH(m.str(2), "\\d+\\.\\d+"_re);
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "stdout");
            ASSERT_EQ(m.str(2), "\"\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "stderr");
            ASSERT_EQ(m.str(2), "\"\"");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "        }");
            // result
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "      ]");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "    }");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "  ],");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "count");
            ASSERT_EQ(m.str(2), "3");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "passes");
            ASSERT_EQ(m.str(2), "1");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "failures");
            ASSERT_EQ(m.str(2), "1");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "errors");
            ASSERT_EQ(m.str(2), "1");
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_MATCH(line, prop_re, m);
            ASSERT_EQ(m.str(1), "time");
            ASSERT_MATCH(m.str(2), "\\d+\\.\\d+"_re);
            ASSERT_TRUE(bool(std::getline(t_ss, line)));
            ASSERT_EQ(line, "}");
        }
        {  // stripped
            t_ss.str("");
            t_ss.clear();
            t_cfg.capture_out = false;
            t_cfg.strip       = true;
            auto uut          = reporter_factory::make<json_reporter>(t_cfg);
            uut->begin_report();
            uut->report(t_ts);
            uut->end_report();
            ASSERT_NOT_LIKE((std::regex_replace(t_ss.str(), std::regex("\"reason\":\".*?\""), "\"reason\":\"\"")),
                            "\\s"_re);
        }
    };

    TEST("markdown_reporter") {
        auto uut = reporter_factory::make<markdown_reporter>(t_cfg);
        uut->begin_report();
        uut->report(t_ts);
        uut->end_report();
        std::smatch m;
        std::string line;
        auto const  test_re{R"(\|(.*?)\|\d+\.\d+ms\|(.*?)\|(.*?)\|(.*?)\|)"_re};

        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_EQ(line, "# Test Report");
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        // testsuite
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_EQ(line, "## testsuite");
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_EQ(line, "|Tests|Successes|Failures|Errors|Time|");
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_EQ(line, "|-|-|-|-|-|");
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, "\\|(\\d+)\\|(\\d+)\\|(\\d+)\\|(\\d+)\\|\\d+\\.\\d+ms\\|"_re, m);
        ASSERT_EQ(m.str(1), "3");
        ASSERT_EQ(m.str(2), "1");
        ASSERT_EQ(m.str(3), "1");
        ASSERT_EQ(m.str(4), "1");
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_EQ(line, "### Tests");
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_EQ(line, "|Name|Time|Status|System-Out|System-Err|");
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_EQ(line, "|-|-|-|-|-|");
        // test1
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, test_re, m);
        ASSERT_EQ(m.str(1), "test1");
        ASSERT_EQ(m.str(2), "PASSED");
        ASSERT_EQ(m.str(3), "");
        ASSERT_EQ(m.str(4), "");
        // test2
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, test_re, m);
        ASSERT_EQ(m.str(1), "test2");
        ASSERT_EQ(m.str(2), "FAILED");
        ASSERT_EQ(m.str(3), "`hello`");
        ASSERT_EQ(m.str(4), "");
        // test3
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, test_re, m);
        ASSERT_EQ(m.str(1), "test3");
        ASSERT_EQ(m.str(2), "ERROR");
        ASSERT_EQ(m.str(3), "");
        ASSERT_EQ(m.str(4), "");
        // result
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_EQ(line, "## Summary");
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_EQ(line, "|Tests|Successes|Failures|Errors|Time|");
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_EQ(line, "|-|-|-|-|-|");
        ASSERT_TRUE(bool(std::getline(t_ss, line)));
        ASSERT_MATCH(line, "\\|(\\d+)\\|(\\d+)\\|(\\d+)\\|(\\d+)\\|\\d+\\.\\d+ms\\|"_re, m);
        ASSERT_EQ(m.str(1), "3");
        ASSERT_EQ(m.str(2), "1");
        ASSERT_EQ(m.str(3), "1");
        ASSERT_EQ(m.str(4), "1");
    };
};

SUITE("test_cmdline_parser"){};

#ifdef TPP_INTERN_SYS_UNIX
#    pragma GCC diagnostic pop
#endif
