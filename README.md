# Test++

[![Linux Build Status](http://badges.herokuapp.com/travis/Jarthianur/TestPlusPlus?env=BADGE=linux&label=linux&branch=master)](https://travis-ci.org/Jarthianur/TestPlusPlus)
[![OSX Build Status](http://badges.herokuapp.com/travis/Jarthianur/TestPlusPlus?env=BADGE=osx&label=osx&branch=master)](https://travis-ci.org/Jarthianur/TestPlusPlus)
[![Windows Build Status](http://badges.herokuapp.com/travis/Jarthianur/TestPlusPlus?env=BADGE=windows&label=windows&branch=master)](https://travis-ci.org/Jarthianur/TestPlusPlus)
[![codecov](https://codecov.io/gh/Jarthianur/TestPlusPlus/branch/master/graph/badge.svg)](https://codecov.io/gh/Jarthianur/TestPlusPlus)
[![BCH compliance](https://bettercodehub.com/edge/badge/Jarthianur/TestPlusPlus?branch=master)](https://bettercodehub.com/)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/2703ed11263b42d9a33f469cc0bc3eb5)](https://www.codacy.com/gh/Jarthianur/TestPlusPlus/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Jarthianur/TestPlusPlus&amp;utm_campaign=Badge_Grade)
[![CodeFactor](https://www.codefactor.io/repository/github/jarthianur/testplusplus/badge)](https://www.codefactor.io/repository/github/jarthianur/testplusplus)

**This is an easy to use, header-only testing framework for C++11/14/17 featuring a simple, yet powerfull API and the capability to parallelize tests using *OpenMP*.**

To use it, just include the all in one [header](https://github.com/Jarthianur/TestPlusPlus/releases/latest) into your builds.

*So why actually writing a new testing framework?*

There are great C++ testing frameworks available out there, but each has its pro's and con's.
When I started to write proper tests for my projects, I couldn't find a testing framework that completely suited my needs.
So I thought, why not just writing my own, which simply has every feature I'd like.
This framework is designed around simplicity and effectiveness, for the user as well as the frameworks code itself.
There is no excessive documentation reading required, or complicated build integration.
Everything is hidden that distracts you from focusing on what really counts - writing qualitative tests.
Of course this framework might not suite all *your* needs, hence feel free to provide suggestions, or request a feature you'd wish to have.
Please have a look at the full [feature set](#feature-set).

## Contents

- [Test++](#test)
  - [Contents](#contents)
  - [Feature Set](#feature-set)
  - [Usage](#usage)
    - [Test Styles](#test-styles)
    - [Scopes and Fixtures](#scopes-and-fixtures)
    - [Floating Point Numbers](#floating-point-numbers)
    - [Regular Expressions](#regular-expressions)
    - [Examples](#examples)
      - [Simple Unit Test](#simple-unit-test)
      - [Behavior Driven Test](#behavior-driven-test)
  - [API](#api)
    - [Tests](#tests)
    - [Reporters](#reporters)
    - [Comparators](#comparators)
    - [Assertions](#assertions)
  - [Parallelization Of Tests](#parallelization-of-tests)
  - [How To Extend](#how-to-extend)
    - [Custom Reporter](#custom-reporter)
    - [Custom Comparator](#custom-comparator)
  - [Contributing](#contributing)

## Feature Set

As a short summary of all features, have a look at this list.

- Single header file for inclusion
- Self registering test suites
- Comparator based assertions
  - equals
  - less than
  - greater than
  - in range (substring, contains)
  - regex match, search
- Report generation in different formats
  - JUnit XML
  - markdown
  - JSON
  - console output
  - support for ANSI colors, and whitespace stripping
- default `main` with
  - commandline parsing
  - glob based inlcude/exclude filters for testsuites
  - report format selection
- **Multithreaded test execution with OpenMP**
- **Output capturing per testcase (even when multithreaded)**
- Unit and behavior-driven test styles
- Compatible compilers
  - gcc
  - clang
  - msvc

## Usage

Just inlude the release *tpp.hpp* header file into your build.
Tests can then be written in source files and simply linked into your test binaries.
In *one* of your test source files call the `TPP_DEFAULT_MAIN` macro.
All tests automatically register themselves, and the rest is done by Test++.
The produced binary allows report selection, filtering etc.
In order to run tests in multiple threads you have to enable OpenMP at compilation (e.g. for gcc add `-fopenmp` flag).
Every output to stdout, or stderr from inside tests is captured per testcase, and can be included in the report.

### Test Styles

Basically there exist two approaches of writing tests.
One is the classic unit test style, where tests are generally designed around an implementation.
The other is behavior driven test style (BDD), where tests are designed around the behavior of an implementation.
No matter which of them you prefer, this framework serves both.
Actually both styles require just a little different syntax.
In the end `DESCRIBE` is just an alias for `SUITE`, same with `IT` and `TEST`.

### Scopes and Fixtures

A testsuite is nothing else than a class definition under the hood, which is instantiated with static lifetime.
Hence the same scoping rules as for usual class definitions apply to testsuites.
Testcases are member functions of their testsuite.
Hence the usual scoping rules for class methods apply to them.

You might know the need for fixtures, basically an instance of a unit under test (UUT), in other contexts.
It is up to you to decide whether testcases should be isolated, or run against a designated fixture.
This framework supports the usage of fixtures, as you can just declare any object at testsuite scope - remember, it's just a member field in the end.
Also it is possible to define functions that will be executed once before and after all testcases, as well as before and after each testcase.
But be carefull when you use these features in multithreaded tests, as there is no additional synchronization happening.
Have a look at the examples, or the [API](#api) to see how this is done exactly.

The use of a testsuite wide UUT instance might also be usefull, if it is expensive to construct and initiate it every time.

### Floating Point Numbers

As floating-point equality comparison relies on a so called epsilon, we need to define such an epsilon.
There is a special comparator for floating point numbers, called F_EQUALS.
It uses the global epsilon value by default, but allows to pass it a certain epsilon value for a single comparison.
If you use this comparator at any point, you must define the global epsilon by invoking `TPP_EPSILON(...)` macro once.
You could use the machine epsilon, but this may lead into false-negative test results, as it could be too accurate.
This epsilon is actually not the epsilon as per definition, but a precision threshold.
For example if you use *0.001* as *epsilon*, *0.100234* and *0.100345* will be considered equals.

### Regular Expressions

This framework provides two comparators for regular expression matching, `MATCH` and `LIKE`.
The first one does a full match, while the second does a search.
When passing a cstring, or string to these comparators a default `std::regex` is used.
To provide better support there are literal operators `"you regex"_re`, and `"your regex"_re_i` for case insensitive matching.
Both operators create regular expressions with *ECMAScript* syntax.
They both accept `std::match_results` in order to provide access to captured groups from the regex.

### Examples

#### Simple Unit Test

```cpp
#include "tpp.hpp"
TPP_EPSILON(0.001)
TPP_DEFAULT_MAIN

using namespace tpp;

SUITE("testSomething") {
    TEST("abc") {
        ASSERT(x+1, EQ(), 11);
        std::cmatch m;
        ASSERT("hello world", MATCH(m), "(hello).*"_re);
        ASSERT_EQ(m.str(1), "hello");
        ASSERT("xyz"s, !IN(), "hello"s);
        int i = 101;
        ASSERT_NOT_NULL(&i);
    }
    TEST("multiple numbers") {
        ASSERT_EQUALS(0, 0);
        ASSERT_ZERO(0.0);
        ASSERT_TRUE(true);
        ASSERT_NOT(.0, FEQ(0.01), 1.0);
        ASSERT(6, IN_RANGE(), std::pair<int, int>(1, 5));
    }
    TEST("ranges") {
        ASSERT('w', IN(), std::string("world"));
        ASSERT(std::string(""), IN(), std::vector<std::string>{""});
    }
    TEST("exceptions") {
        auto e = ASSERT_THROWS(throw std::logic_error("abc"), std::logic_error);
        ASSERT_EQ(std::string(e.what()), "abc");
        auto r = ASSERT_NOTHROW(return 1);
        ASSERT_EQ(r, 1);
    }
};
```

#### Behavior Driven Test

```cpp
#include "tpp.hpp"

using tpp::GT;

TPP_DEFAULT_MAIN

class MyClass {
    public:
        int i = 0;
        bool function() { return true; }
};

DESCRIBE("testMyClass") {
    MyClass my;

    SETUP() {
        my.i = 1;
    }
    AFTER_EACH() {
        my.i++;
    }

    IT("should return true") {
        ASSERT_TRUE(my.function());
    }
    IT("should hold i larger than 0") {
        ASSERT(my.i, GT(), 0);
    }
};
```

## API

### Tests

| Macro                   | Arguments             | Description                                                                             |
| ----------------------- | --------------------- | --------------------------------------------------------------------------------------- |
| SUITE, DESCRIBE         | description (cstring) | Create a testsuite.                                                                     |
| SUITE_PAR, DESCRIBE_PAR | description (cstring) | Create a testsuite, where all tests will get executed concurrently in multiple threads. |
| TEST, IT                | description (cstring) | Create a testcase in a testsuite.                                                       |
| SETUP                   |                       | Define a function, which will be executed once before all testcases.                    |
| TEARDOWN                |                       | Define a function, which will be executed once after all testcases.                     |
| BEFORE_EACH             |                       | Define a function, which will be executed before each testcase.                         |
| AFTER_EACH              |                       | Define a function, which will be executed after each testcase.                          |

### Reporters

| Format         | Command Line Switch | Supported Options |
| -------------- | ------------------- | ----------------- |
| Console output |                     | -c -o             |
| JUnit XML      | --xml               | -s -o             |
| Markdown       | --md                | -o                |
| JSON           | --json              | -c -s -o          |

### Comparators

**Note:** Assertions are based on comparators. Every comparator provides a negation operator `!`, which allows the logical negation of the actual comparison. For example `ASSERT(1, !EQ, 1)`.

| Comparator                | Description                                                                                                                                          |
| ------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| EQUALS, EQ                | Compare two values to be equal.                                                                                                                      |
| F_EQUALS, FEQ             | Compare two floating point numbers to be equal. Optionally takes a precision threshold as argument, and uses the global epsilon otherwise.           |
| GREATER_THAN, GREATER, GT | Compare one value to be greater than another.                                                                                                        |
| LESS_THAN, LESS, LT       | Compare one value to be less than another.                                                                                                           |
| IN_RANGE, IN              | Check a value to be in range of any container. This means for containers (C++ "ranges") to contain the value and for strings to contain a substring. |
| MATCH                     | Match a given string to a regular expression. Optionally takes a std::match_results as argument to store results into.                               |
| LIKE                      | Search a regular expression in a given string. Optionally takes a std::match_results as argument to store results into.                              |

### Assertions

| Assertion       | Parameters          | Description                                                                                                                                                                                                     |
| --------------- | ------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| ASSERT          | VALUE, COMP, EXPECT | Assert successfull comparison of VALUE and EXPECT with COMP.                                                                                                                                                    |
| ASSERT_NOT      | VALUE, COMP, EXPECT | Assert with negated comparison. Equivalent to `!COMP`.                                                                                                                                                          |
| ASSERT_EQ       | VALUE, EXPECT       | Assert using *EQUALS* comparator.                                                                                                                                                                               |
| ASSERT_TRUE     | VALUE               | Assert VALUE to be *true*.                                                                                                                                                                                      |
| ASSERT_FALSE    | VALUE               | Assert VALUE to be *false*.                                                                                                                                                                                     |
| ASSERT_NOT_NULL | VALUE               | Assert VALUE to be not *nullptr*.                                                                                                                                                                               |
| ASSERT_NULL     | VALUE               | Assert VALUE to be *nullptr*.                                                                                                                                                                                   |
| ASSERT_ZERO     | VALUE               | Assert VALUE to be *0*.                                                                                                                                                                                         |
| ASSERT_THROWS   | STMT, TYPE          | Assert STMT to throw an exception of TYPE. Multiple statements can be split by `;`. Returns the instance of TYPE if caught.                                                                                     |
| ASSERT_NOTHROW  | STMT                | Assert STMT not to throw any exception.  Multiple statements can be split by `;`. Returns the return value of STMT, if there is any.                                                                            |
| ASSERT_RUNTIME  | STMT, MILLIS        | Assert STMT to run in MILLIS milliseconds at maximum. The statement is not interrupted, if the time exceeds MILLIS. Multiple statements can be split by `;`. Returns the return value of STMT, if there is any. |

## Parallelization Of Tests

This testing framework serves the capability of parallelizing tests, using OpenMP. Actually it is not really parallel, but concurrent.
Nevertheless, it may reduce test durations massively.
Keep in mind that tests, running concurrently, *must* be completely independent from each other.
The same rules for usual multithreading apply here, to not produce dataraces, or deadlocks.
As long as testcases do not share any data, it is completely threadsafe.
If testcases share data, like fixtures, you have to take care of synchronization, meaning that all invokations on your shared data needs to be threadsafe.
This also applies to `BEFORE_EACH` and `AFTER_EACH` definitions, while `SETUP` and `TEARDOWN` are executed synchronous.
You may also use parallel tests to test a components threadsafety - actually not a proper way of testing threadsafety.
Also consider, spawning threads has some overhead.
Thus there is no point in running just a few, anyway fast tests concurrently.
*Usually* the threadpool is kept alive in the background.
So if you use parallel testsuites once, don't be afraid to use them wherever you can, even for short tests, as there is not much more overhead.

## How To Extend

### Custom Reporter

To add a new reporter just implement the [reporter](include/report/reporter.hpp) interface. Have a look at the preimplemented reporters, how this is exactly done.

### Custom Comparator

To add a new comparator you basically just need to invoke two macros.
For example:

```c
COMPARATOR(pred, "predecessor of", actual_value + 1 == expected_value)
PROVIDE_COMPARATOR(pred, PRE)
```

This will create a generic comparator for you. The syntax is as follows:
`COMPARATOR(NAME, CONSTRAINT, PREDICATE)`, where *NAME* is the comparator name.
*CONSTRAINT* is a cstring representing what it does and is used for reporting.
*PREDICATE* is the actual comparison.

## Contributing

Contribution to this project is always welcome.
