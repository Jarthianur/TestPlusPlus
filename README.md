# Test++

[![Linux Build Status](http://badges.herokuapp.com/travis/Jarthianur/TestPlusPlus?env=BADGE=linux&label=linux&branch=master)](https://travis-ci.org/Jarthianur/TestPlusPlus)
[![OSX Build Status](http://badges.herokuapp.com/travis/Jarthianur/TestPlusPlus?env=BADGE=osx&label=osx&branch=master)](https://travis-ci.org/Jarthianur/TestPlusPlus)
[![Windows Build Status](http://badges.herokuapp.com/travis/Jarthianur/TestPlusPlus?env=BADGE=windows&label=windows&branch=master)](https://travis-ci.org/Jarthianur/TestPlusPlus)
[![codecov](https://codecov.io/gh/Jarthianur/TestPlusPlus/branch/master/graph/badge.svg)](https://codecov.io/gh/Jarthianur/TestPlusPlus)
[![BCH compliance](https://bettercodehub.com/edge/badge/Jarthianur/TestPlusPlus?branch=master)](https://bettercodehub.com/)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/2703ed11263b42d9a33f469cc0bc3eb5)](https://www.codacy.com/gh/Jarthianur/TestPlusPlus/dashboard?utm_source=github.com&utm_medium=referral&utm_content=Jarthianur/TestPlusPlus&utm_campaign=Badge_Grade)
[![CodeFactor](https://www.codefactor.io/repository/github/jarthianur/testplusplus/badge)](https://www.codefactor.io/repository/github/jarthianur/testplusplus)

**This is an easy to use, header-only testing framework for C++11/14/17 featuring a simple, yet powerfull API and the capability to parallelize tests using _OpenMP_.**

To use it, just include the all in one [header](https://github.com/Jarthianur/TestPlusPlus/releases/latest) into your builds.
If you want to include it via CMake, have a look at [Usage](#usage).

_So why actually writing a new testing framework?_

There are great C++ testing frameworks available out there, but each has its pro's and con's.
When I started to write proper tests for my projects, I couldn't find a testing framework that completely suited my needs.
So I thought, why not just writing my own one which simply has every feature I'd like.
This framework is designed around simplicity and effectiveness in usage as well as the frameworks code itself.
There is no excessive documentation reading required or complicated build integration.
Everything is hidden that distracts you from focusing on what really counts - writing qualitative tests.
Of course this framework might not suite all _your_ needs, hence feel free to provide suggestions, or request a feature you'd wish to have.
Please have a look at the full [feature set](#feature-set).

## Contents

<!-- TOC -->

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
  - [Comparators](#comparators)
  - [Assertions](#assertions)
- [Parallelization Of Tests](#parallelization-of-tests)
- [Contributing](#contributing)
<!-- /TOC -->

## Feature Set

As a short summary of all features, have a look at this list.

- Single header file for inclusion
- Self registering test suites
- Comparator based assertions
  - equals
  - less than
  - greater than
  - in range (strings, containers)
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

Just inlude the release _tpp.hpp_ header file into your build.
Alternatively, for CMake integration add the following parts to your CMakeLists.txt:

```
add_subdirectory(path/to/TestPlusPlus EXCLUDE_FROM_ALL)
target_link_libraries(... tpp)
```

Tests can then be written in source files and simply linked into your test binaries.
In _one_ of your test source files call the `TPP_DEFAULT_MAIN` macro.
All tests automatically register themselves, and the rest is done by Test++.
The produced binary allows report selection, filtering etc.
In order to run tests in multiple threads you have to enable OpenMP at compilation (e.g. for gcc add `-fopenmp` flag).
Every output to stdout or stderr from inside tests is captured per testcase and can be included in the report.

To run your tests, run the resulting binary.

```
$ ./my-test --help
Usage: ./my-test [OPTIONS] [filename]
Default is to report to standard-out in an informative text format (using console-reporter).

OPTIONS:
  --help: Print this message and exit.
  --xml : Report in JUnit-like XML format.
  --md  : Report in markdown format.
  --json: Report in json format.
  -c    : Use ANSI colors in report, if supported by reporter.
  -s    : Strip unnecessary whitespaces from report.
  -o    : Report captured output from tests, if supported by reporter.

  Multiple filters are possible, but includes and excludes are mutually exclusive.
  Patterns may contain * as wildcard.

  -e <pattern> : Exclude testsuites with names matching pattern.
  -i <pattern> : Include only testsuites with names matching pattern.
```

### Test Styles

Basically there exist two approaches of writing tests.
One is the classic unit test style, where tests are generally designed around an implementation.
The other is behavior driven test style (BDD), where tests are designed around the behavior of an implementation.
No matter which of them you prefer, this framework serves both.
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

### Floating Point Numbers

As floating-point equality comparison relies on a so called epsilon, we need to define such an epsilon.
The global epsilon value is used by default, but it's possible to pass a certain epsilon value for a single comparison.
This epsilon is actually not the epsilon as per definition, but a precision threshold.
You could use the machine epsilon, but this may lead to false-negative test results, as it could be too accurate.
If you compare floating-point numbers for equality at any point, you must define the global epsilon by invoking `TPP_EPSILON(epsilon)` macro once.
For example if you use _0.001_ as epsilon, _0.100234_ and _0.100345_ will be considered equals.

### Regular Expressions

This framework provides two comparators for regular expression matching, `MATCH` and `LIKE`.
The first one does a full match, while the second does a search.
When passing a cstring, or string to these comparators a default `std::regex` is used.
To provide better support there are literal operators `"you regex"_re`, and `"your regex"_re_i` for case insensitive matching.
Both operators create regular expressions with _ECMAScript_ syntax.
The comparators optionally accept `std::match_results` in order to provide access to captured groups from the regex.

### Examples

#### Simple Unit Test

```cpp
#include "tpp.hpp"
TPP_EPSILON(0.001)
TPP_DEFAULT_MAIN

using namespace tpp;

SUITE("testSomething") {
    TEST("abc") {
        ASSERT(x+1, EQ, 11);
        std::cmatch m;
        ASSERT("hello world", MATCH, "(hello).*"_re, m);
        ASSERT_EQ(m.str(1), "hello");
        ASSERT_NOT("xyz"s, IN, "hello"s);
        int i = 101;
        ASSERT_NOT_NULL(&i);
    }
    TEST("multiple numbers") {
        ASSERT_EQUALS(0, 0);
        ASSERT_TRUE(true);
        ASSERT_NOT_EQ(.0, 1.0, 0.01);
        ASSERT_IN(6, std::pair<int, int>(1, 5));
    }
    TEST("ranges") {
        ASSERT('w', IN, std::string("world"));
        ASSERT_IN(std::string(""), std::vector<std::string>{""});
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
        ASSERT(my.i, GT, 0);
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

### Comparators

| Comparator | Description                                                                                   | Optional Arguments                          |
| ---------- | --------------------------------------------------------------------------------------------- | ------------------------------------------- |
| EQ         | Compare two values to be equal.                                                               | Accepts an epsilon value.                   |
| GT         | Compare one value to be greater than another.                                                 |                                             |
| LT         | Compare one value to be less than another.                                                    |                                             |
| IN         | Check a container (C++ "ranges") to contain the value and for strings to contain a substring. |                                             |
| MATCH      | Match a given string to a regular expression.                                                 | Accepts a `std::match_result` as reference. |
| LIKE       | Search a regular expression in a given string.                                                | Accepts a `std::match_result` as reference. |

### Assertions

| Assertion        | Parameters   | Description                                                                                                                                |
| ---------------- | ------------ | ------------------------------------------------------------------------------------------------------------------------------------------ |
| ASSERT           | V, C, E, ... | Assert successfull comparison of _V_ and _E_ with _C_, pass further arguments to _C_.                                                      |
| ASSERT_NOT       | V, C, E, ... | Like _ASSERT_, but with negated result.                                                                                                    |
| ASSERT_EQ        | V, E, ...    | Assert using _EQ_ comparator.                                                                                                              |
| ASSERT_NOT_EQ    | V, E, ...    | Assert using _EQ_ comparator, but with negated result.                                                                                     |
| ASSERT_LT        | V, E         | Assert using _LT_ comparator.                                                                                                              |
| ASSERT_NOT_LT    | V, E         | Assert using _LT_ comparator, but with negated result.                                                                                     |
| ASSERT_GT        | V, E         | Assert using _GT_ comparator.                                                                                                              |
| ASSERT_NOT_GT    | V, E         | Assert using _GT_ comparator, but with negated result.                                                                                     |
| ASSERT_IN        | V, E         | Assert using _IN_ comparator.                                                                                                              |
| ASSERT_NOT_IN    | V, E         | Assert using _IN_ comparator, but with negated result.                                                                                     |
| ASSERT_MATCH     | V, E, ...    | Assert using _MATCH_ comparator.                                                                                                           |
| ASSERT_NOT_MATCH | V, E, ...    | Assert using _MATCH_ comparator, but with negated result.                                                                                  |
| ASSERT_LIKE      | V, E, ...    | Assert using _LIKE_ comparator.                                                                                                            |
| ASSERT_NOT_LIKE  | V, E, ...    | Assert using _LIKE_ comparator, but with negated result.                                                                                   |
| ASSERT_TRUE      | V            | Assert _V_ to be _true_.                                                                                                                   |
| ASSERT_FALSE     | V            | Assert _V_ to be _false_.                                                                                                                  |
| ASSERT_NULL      | V            | Assert _V_ to be _nullptr_.                                                                                                                |
| ASSERT_NOT_NULL  | V            | Assert _V_ to be not _nullptr_.                                                                                                            |
| ASSERT_THROWS    | S, E         | Assert _S_ to throw _T_. Returns the instance of _T_.                                                                                      |
| ASSERT_NOTHROW   | S            | Assert _S_ not to throw. Returns the return value of _S_ if there is any.                                                                  |
| ASSERT_RUNTIME   | S, M         | Assert _S_ to finish in _M_ milliseconds. _S_ is not interrupted if the time exceeds _M_. Returns the return value of _S_ if there is any. |

## Parallelization Of Tests

This testing framework serves the capability of parallelizing tests using OpenMP. Actually it is not really parallel, but concurrent.
Nevertheless, it may reduce test durations massively.
Keep in mind that tests running concurrently must be completely independent from each other.
The same rules for usual multithreading apply here, to not produce dataraces or deadlocks.
As long as testcases do not share any data, it is completely threadsafe.
If testcases share data, you have to take care of synchronization.
This also applies to `BEFORE_EACH` and `AFTER_EACH` definitions, while `SETUP` and `TEARDOWN` are executed synchronous.
Also consider, spawning threads has some overhead.
Hence there is no point in running just a few fast tests concurrently.
Usually the threadpool is kept alive in the background.
So if you use parallel testsuites once, don't be afraid to use them wherever you can, even for short tests as there is not much more overhead.

## Contributing

Contribution to this project is always welcome.
