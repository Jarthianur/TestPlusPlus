# simple-cpp-test-framework

[![Build Status](https://travis-ci.org/Jarthianur/simple-cpp-test-framework.svg?branch=master)](https://travis-ci.org/Jarthianur/simple-cpp-test-framework)
[![Linux Build Status](http://badges.herokuapp.com/travis/Jarthianur/simple-cpp-test-framework?env=BADGE=linux&label=linux&branch=master)](https://travis-ci.org/Jarthianur/simple-cpp-test-framework)
[![OSX Build Status](http://badges.herokuapp.com/travis/Jarthianur/simple-cpp-test-framework?env=BADGE=osx&label=osx&branch=master)](https://travis-ci.org/Jarthianur/simple-cpp-test-framework)
[![Windows Build Status](http://badges.herokuapp.com/travis/Jarthianur/simple-cpp-test-framework?env=BADGE=windows&label=windows&branch=master)](https://travis-ci.org/Jarthianur/simple-cpp-test-framework)
[![codecov](https://codecov.io/gh/Jarthianur/simple-cpp-test-framework/branch/master/graph/badge.svg)](https://codecov.io/gh/Jarthianur/simple-cpp-test-framework)
[![BCH compliance](https://bettercodehub.com/edge/badge/Jarthianur/simple-cpp-test-framework?branch=master)](https://bettercodehub.com/)

**This is a simple header-only testing framework for C++11/14/17.**
Featuring great **extendability** and an **easy, but powerful and modular** API.
Additionally it serves the capability to **parallelize** tests, using *OpenMP*.

To use it, just include the all in one [header](sctf.hpp) into your builds.

*So why actually writing a new testing framework?*

There are great C++ testing frameworks available out there, but each has its pro's and con's.
When I started to write proper tests for my projects, I couldn't find a testing framework that completely suited my needs.
So I thought, why not just writing my own testing framework, which simply has every feature I'd like.
As a side effect this project was - and is still - a great opportunity to improve my C++ skills.
This framework is designed around simplicity and effectiveness, for the user as well as the frameworks code itself.
There is no excessive documentation reading required, or complicated build integration.
Everything is hidden that distracts you from focusing on what really counts, writing qualitative tests.
Of course this framework might not suite your needs completely, hence feel free to provide suggestions, or request a feature you'd like.
Please have a look at the full [feature set](#feature-set).

## Contents

- [simple-cpp-test-framework](#simple-cpp-test-framework)
  - [Contents](#contents)
  - [Feature Set](#feature-set)
  - [Usage](#usage)
    - [Floats](#floats)
    - [Test Styles](#test-styles)
    - [Fixtures](#fixtures)
    - [Examples](#examples)
      - [Simple Unit Test](#simple-unit-test)
      - [Behavior Driven Test](#behavior-driven-test)
  - [API](#api)
    - [Tests](#tests)
    - [Reporters](#reporters)
    - [Comparators](#comparators)
    - [Assertions](#assertions)
  - [Some Words About Parallelization](#some-words-about-parallelization)
  - [Extending](#extending)
    - [of reporters](#of-reporters)
    - [of comparators](#of-comparators)
    - [Stringify values](#stringify-values)
  - [Contributing](#contributing)
      - [Footnote](#footnote)

## Feature Set

As a short summary of all features, have a look at this list.

+ Single header file for inclusion
+ Self registering test suites
+ Comparator based assertions
  + equals
  + unequals
  + less than
  + greater than
  + in range (substring, contains)
+ Report generation in different formats
  + JUnit XML
  + markdown
  + console output
+ **Multithreaded test execution with OpenMP**
+ **Output capturing per testcase (even when multithreaded)**
+ Unit and behavior-driven test styles
+ Compatible compilers
  + gcc
  + clang
  + msvc

## Usage

Just inlude the *sctf.hpp* header file into your build.
Tests can then be written in source files and simply linked into your test binary.
If you prefer, you can of course write tests in header files and include them in your main source file.
To run and report all tests, just create a reporter and call its `report()` function in your `main()`, as seen in the examples.
Alternatively invoke the `SCTF_DEFAULT_MAIN(...)` macro in one of your source files and pass it a call to any reporter factory method.
In order to run tests in multiple threads, you have to enable OpenMP at compilation (e.g. for gcc add `-fopenmp` flag).

### Floats

As floating-point comparison relies on a so called epsilon, we use the machine epsilon by default. But this may lead into false-negative test results, as it could be too accurate. In order to use a custom epsilon, there are two ways to achieve this. First, you can provide a macro in each compilation unit, which is called `SCTF_EPSILON` with a satisfying value (like 0.000001). A compiler invocation could look like "`g++ -std=c++0x test.cpp -fopenmp -DSCTF_EPSILON=0.000001`". Or you provide it before including *sctf.hpp* like in the [example](#example) below. The second way, is to provide it as an extern variable. Therefor define `SCTF_EXTERN_EPSILON` before including *sctf.hpp* and than call the `SCTF_SET_EPSILON` macro with a satisfying value in the compilation unit (cpp file). The latter one allows more fine grained control over the required epsilon value.

### Test Styles

### Fixtures

### Examples

#### Simple Unit Test

```cpp
#define SCTF_EXTERN_EPSILON
#include "sctf.hpp"
SCTF_SET_EPSILON(0.001)
SCTF_DEFAULT_MAIN(create_xml_reporter())

SUITE(testSomething) {
    TEST("abc") {
        ASSERT(x+1, EQ, 11);
        ASSERT_FALSE(false);
        ASSERT("xyz"s, !IN, "hello"s);
        int i = 101;
        ASSERT_NOT_NULL(&i);
    }
    TEST("multiple numbers") {
        ASSERT_EQUALS(0, 0);
        ASSERT_ZERO(0.0);
        ASSERT_TRUE(true);
        ASSERT_NOT(.0, EQUALS, 1.0);
        ASSERT(6, IN_RANGE, std::pair<int, int>(1, 5));
    }
    TEST("ranges") {
        ASSERT('w', IN, std::string("world"));
        ASSERT(std::string(""), IN, std::vector<std::string>{""});
    }
    TEST("exceptions") {
        ASSERT_THROWS(throw std::logic_error(""), std::logic_error);
    }
};
```

#### Behavior Driven Test

```cpp
#define SCTF_EPSILON 0.00001
#include "sctf.hpp"
SCTF_DEFAULT_MAIN(create_xml_reporter("test_report.xml"))

class MyClass {
    public:
    int i = 0;
    bool function() { return true; }
};

DESCRIBE(testMyClass) {
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

| Call                    | Arguments   | Description                                                                                                                         |
| ----------------------- | ----------- | ----------------------------------------------------------------------------------------------------------------------------------- |
| SUITE, DESCRIBE         | identifier  | Create a testsuite. The given identifier must be globally unique.                                                                   |
| SUITE_PAR, DESCRIBE_PAR | identifier  | Create a testsuite. Tests in there will be executed concurrently in multiple threads. The given identifier must be globally unique. |
| TEST, IT                | description | Create a testcase in a testsuite. The given description is a cstring.                                                               |
| SETUP                   |             | Define a function, which will be executed once before all testcases. Thrown exceptions will get ignored.                            |
| TEARDOWN                |             | Define a function, which will be executed once after all testcases. Thrown exceptions will get ignored.                             |
| BEFORE_EACH             |             | Define a function, which will be executed before each testcase. Thrown exceptions will get ignored.                                 |
| AFTER_EACH              |             | Define a function, which will be executed after each testcase. Thrown exceptions will get ignored.                                  |

### Reporters

| Format            | Factory Method                                                                                                                                                            | Description |
| ----------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| console_reporter  | Produces plain text, which is aimed for human-readable console output. Optionally ANSI colors can be enabled. Allows reporting of captured output from stdout and stderr. |
| xml_reporter      | Produces JUnit like XML format.                                                                                                                                           |
| markdown_reporter | Produces a minimalistic report in markdown format.                                                                                                                        |

### Comparators

**Note:** While the *comparator* names the function itself, the *shortwrite* is what you actually write in code. Assertions are based on comparators. Every comparator provides a negation operator `!`, which allows the logical negation of the actual comparison. For example `ASSERT(1, !EQ, 1)` and `ASSERT(1, NE, 1)` are logically equivalent.

| Comparator   | Shortwrites               | Description                                                                                                                                          |
| ------------ | ------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| equals       | EQUALS, EQ                | Compare two values to be equal, using `operator ==`.                                                                                                 |
| unequals     | UNEQUALS, NE              | Compare two values to be not equal, using `operator !=`.                                                                                             |
| greater_than | GREATER_THAN, GREATER, GT | Compare one value to be greater than another, using `operator >`.                                                                                    |
| less_than    | LESS_THAN, LESS, LT       | Compare one value to be less than another, using `operator <`.                                                                                       |
| in_range     | IN_RANGE, IN              | Check a value to be in range of any container. This means for containers (C++ "ranges") to contain the value and for strings to contain a substring. |

### Assertions

**Note:** VALUE is the actual value you want to check. EXPECT is the expected value. COMP is the comparator. FUNC is one statement, or multiple split by `;`.

| Assertion          | Parameters          | Description                                                                                                              |
| ------------------ | ------------------- | ------------------------------------------------------------------------------------------------------------------------ |
| ASSERT             | VALUE, COMP, EXPECT | Common assert. Assert successfull comparison of VALUE and EXPECT with COMP.                                              |
| ASSERT_NOT         | VALUE, COMP, EXPECT | Same as *assert*, but with negated comparison.                                                                           |
| ASSERT_EQUALS      | VALUE, EXPECT       | Wrapper for *assert* using *EQUALS* comparator.                                                                          |
| ASSERT_TRUE        | VALUE               | Assert VALUE to be *true*.                                                                                               |
| ASSERT_FALSE       | VALUE               | Assert VALUE to be *false*.                                                                                              |
| ASSERT_NOT_NULL    | VALUE               | Assert VALUE not to be *nullptr*.                                                                                        |
| ASSERT_NULL        | VALUE               | Assert VALUE to be *nullptr*.                                                                                            |
| ASSERT_ZERO        | VALUE               | Assert VALUE to be *0*, where the type of *0* will match VALUE's.                                                        |
| ASSERT_THROWS      | FUNC, TYPE          | Assert FUNC to throw an exception of TYPE.                                                                               |
| ASSERT_NOTHROW     | FUNC                | Assert FUNC not to throw any exception.                                                                                  |
| ASSERT_PERFORMANCE | FUNC, MILLIS        | Assert FUNC to run in MILLIS milliseconds at maximum. The FUNC statement is not interrupted, if the time exceeds MILLIS. |

## Some Words About Parallelization

This testing framework serves the capability of parallelizing tests, using OpenMP. Actually it is not really parallel, but concurrent.
Nevertheless, it may reduce test durations massively.
Keep in mind that tests, running concurrently, *must* be completely independent from each other.
Also consider, spawning threads has some overhead. Thus there is no point in running just a few, anyway fast, tests concurrently.

## Extending

### of reporters

To add a new reporter just implement the [reporter](include/reporter/reporter.hpp) interface. Have a look at the preimplemented reporters, how this is exactly done.

### of comparators

To add a new comparator you basically just need to invoke two macros.
For example:

```c
COMPARATOR(pred, "predecessor of", value + 1 == expect)
PROVIDE_COMPARATOR(pred, PRE)
```

This will create a generic comparator function for you. The syntax is as follows:
`COMPARATOR(NAME, CONSTRAINT, PREDICATE)`, where *NAME* is the function name. *CONSTRAINT* is a string representing what it does and is used for reporting. *PREDICATE* is the actual comparison.
If you need some more complex comparators, have a look at the [in_range](include/comparator/inrange.hpp) to see how it is implemented.

To specialize a comparator for custom types, which do not provide a respective operator, just specialize the comparator template. For example:

```cpp
namespace sctf {
namespace _ {
    template<>
    comparison equals<Custom>(const Custom& value, const Custom& expect)
    {
        return value.hash() == expect.hash()
               ? SUCCESS
               : comparison(equals_comp_str, to_string(value),
                            to_string(expect));
    }
}
}
```

### Stringify values

To allow proper reporting, every asserted value/type needs to be stringified somehow. This framework has an approach to convert every value to string if possible, else the typename is used. If you need to handle custom types in a certain way, just specialize the [to_string](include/common/stringify.hpp) template. For example:

```cpp
namespace sctf
{
namespace _
{
    template<>
    inline std::string to_string<Custom>(const Custom& arg)
    {
        return arg.get_id();
    }
}
}
```

## Contributing

Contribution to this project is always welcome.

#### Footnote

I have implemented this framework to test my own C++ projects with a fundamental and extendable API. Nevertheless anybody, finding this framework useful, may use, or even extend and contribute to it.
