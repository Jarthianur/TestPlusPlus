# simple-cpp-test-framework

[![Build Status](https://travis-ci.org/Jarthianur/simple-cpp-test-framework.svg?branch=master)](https://travis-ci.org/Jarthianur/simple-cpp-test-framework)
[![Linux Build Status](http://badges.herokuapp.com/travis/Jarthianur/simple-cpp-test-framework?env=BADGE=linux&label=linux&branch=master)](https://travis-ci.org/Jarthianur/simple-cpp-test-framework)
[![OSX Build Status](http://badges.herokuapp.com/travis/Jarthianur/simple-cpp-test-framework?env=BADGE=osx&label=osx&branch=master)](https://travis-ci.org/Jarthianur/simple-cpp-test-framework)
[![Windows Build Status](http://badges.herokuapp.com/travis/Jarthianur/simple-cpp-test-framework?env=BADGE=windows&label=windows&branch=master)](https://travis-ci.org/Jarthianur/simple-cpp-test-framework)
[![codecov](https://codecov.io/gh/Jarthianur/simple-cpp-test-framework/branch/master/graph/badge.svg)](https://codecov.io/gh/Jarthianur/simple-cpp-test-framework)
[![BCH compliance](https://bettercodehub.com/edge/badge/Jarthianur/simple-cpp-test-framework?branch=master)](https://bettercodehub.com/)

**This is a simple header-only unit testing framework for C++11/14/17.**
Featuring great **extendability** and an **easy, but powerful and modular** API. **Test reports** are generated in a specified format, according to the chosen reporter. Additionally it serves the capability to **parallelize** tests, using *OpenMP*.

*So why actually writing a new unit-testing framework?* Except for the reason of learning how to do, I tried a modular approach instead of just throwing some assertions into magic macros. Yes this framework utilizes macros too, but they are neither required, nor part of the program logic. They just reduce the workload of typing and wrap stuff in a usefull way.
Even test code should be regular code and also look like that. The next point is extendability. You will see later how easy it is to define new, or modify existing logic of this framework, to adjust it to your needs. There is no need to provide an ultra large allmighty framework, as some basic stuff is commonly enough and every project specific logic can be added easily.

## Contents

- [simple-cpp-test-framework](#simple-cpp-test-framework)
  - [Contents](#contents)
  - [Basic Architecture](#basic-architecture)
  - [Some Words About Parallelization](#some-words-about-parallelization)
  - [Feature Set](#feature-set)
    - [Reporters](#reporters)
    - [Testsuites](#testsuites)
    - [Comparators](#comparators)
    - [Assertions](#assertions)
    - [Test Modules](#test-modules)
  - [Usage](#usage)
    - [Floats](#floats)
    - [Example (functional)](#example-functional)
    - [Example (OO)](#example-oo)
  - [Extending](#extending)
    - [of reporters](#of-reporters)
    - [of comparators](#of-comparators)
    - [Stringify values](#stringify-values)
  - [Contributing](#contributing)
      - [Footnote](#footnote)

## Basic Architecture

The great extendability comes from its simple structure and modularity. There are two approaches, functional and object oriented. There is the runner, which runs the testsuites. The testsuites contain testcases and the testcases contain any logic plus assertions. Then there are reporters, which generate a report at the end. Assertions utilize comparators, or do their internal checks. For the functional approach, just describe a testsuite and add tests to it. For the OO approach define a test module.


## Some Words About Parallelization

This testing framework serves the capability of parallelizing tests, using OpenMP. It may reduce test durations massively. Nevertheless this feature should be used carefully.
That means tests, running in parallel, *must* be completely independent from each other. But this fact may also be used to test a components' threadsafety.
Also consider, spawning threads has some overhead. Thus there is no point in running just a few, anyway fast, tests in parallel.

## Feature Set

### Reporters

**Note:** Every reporter has its factory method, which is always like `createNAME(...)`, where *NAME* is the reporter class name.

| Reporter           | Description                                                                                                                                                                         |
| ------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| plaintext_reporter | Produces plain text, which is aimed for human-readable console output. Optionally ANSI colors can be enabled. Also optionally is reporting of captured output to stdout and stderr. |
| xml_reporter       | Produces JUnit like XML format. The result may be published to any visualizer, e.g. in a jenkins environment.                                                                       |
| html_reporter      | Produces a single HTML file, which is a minimal standalone website showing the test results more or less beautiful.                                                                 |

### Testsuites

**Note:** The argument *runner* of all *describe* functions is always the instance where to register the *TestSuite*. It is optional if you just need one. The argument *func* of all *test*,*setup*,*before*,*after* methods is always a void function without arguments, preferably a lambda expression. Calls to those methods are chainable. It is possible to add tests after the runner has run, thus run tests partially, and run it again. Already executed tests will be skipped.

| Call                 | Arguments             | Description                                                                                                                                          | Example                                             |
| -------------------- | --------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------- |
| describe             | name, context, runner | Describe a testsuite with *name* and *context*, where context defaults to "main".                                                                    | `describe("ts1", "Component")...`                   |
| describe\<T>         | name, runner          | Describe a testsuite with *name*. The context is taken as the class-/typename of T.                                                                  | `describe<Component>("ts2")...`                     |
| describeParallel     | name, context, runner | Describe a testsuite with *name* and *context*, where context defaults to an empty string. Tests inside this testsuite will be executed in parallel. | `describeParallel("ts3", "Component")...`           |
| describeParallel\<T> | name, runner          | Describe a testsuite with *name*. The context is taken as the class-/typename of T. Tests inside this testsuite will be executed in parallel.        | `describeParallel<Component>("ts2")...`             |
| test                 | name, context, func   | Create a testcase on a testsuite with *name* and *context*.                                                                                          | `...->test("test1", "Component::func", []{...})...` |
| test                 | name, func            | Create a testcase on a testsuite with *name*, the context is inherited from the testsuite.                                                           | `...->test("test2", []{...})...`                    |
| test\<T>             | name, func            | Create a testcase on a testsuite with *name*. The context is taken as the class-/typename of T.                                                      | `...->test<Component>("test3", []{...})...`         |
| setup                | func                  | Set a *setup* function, which will be executed once before all testcases. Exceptions thrown by the given function will get ignored.                  | `...->setup([&]{ x = 10; ... })...`                 |
| before               | func                  | Set a *pre-test* function, which will be executed before each testcase. Exceptions thrown by the given function will get ignored.                    | `...->before([&]{ x = 10; ... })...`                |
| after                | func                  | Set a *post-test* function, which will be executed after each testcase. Exceptions thrown by the given function will get ignored.                    | `...->after([&]{ x = 10; ... })...`                 |

### Comparators

**Note:** While the *comparator* names the function itself, the *shortwrite* is what you actually write in code.

| Comparator   | Shortwrite(s) | Description                                                                                                                                          |
| ------------ | ------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| equals       | EQUALS, EQ    | Compare two values to be equal, using `operator ==`.                                                                                                 |
| unequals     | UNEQUALS, NE  | Compare two values not to be equal, using `operator !=`.                                                                                             |
| greater_than | GREATER, GT   | Compare one value to be greater than another, using `operator >`.                                                                                    |
| less_than    | LESS, LT      | Compare one value to be less than another, using `operator <`.                                                                                       |
| in_range     | IN_RANGE, IN  | Check a value to be in range of any container. This means for containers (C++ "ranges") to contain the value and for strings to contain a substring. |

### Assertions

**Note:** VALUE means the actual value you want to check. EXPECT means the expected value. COMP means the comparator. TYPE means a certain type, or class. FUNC means a function call, or instruction and is wrapped in a lambda with captions as reference. Hence FUNC can be a single instruction, or multiple split by `;`.

| Assertion         | Parameters                | Description                                                                                                         | Example                                                          |
| ----------------- | ------------------------- | ------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| assert            | VALUE, COMP, EXPECT       | Common assert. Assert successfull comparison of VALUE and EXPECT with COMP.                                         | `assert(1, LT, 1);`                                              |
| assertT           | VALUE, COMP, EXPECT, TYPE | Same as *assert*, but parameters are specialized to TYPE, which allows implicit conversions.                        | `assertT("hell", IN, "hello", std::string);`                     |
| assertEquals      | VALUE, EXPECT             | Wrapper for *assert* using *EQUALS* comparator.                                                                     | `assertEquals(1, 1);`                                            |
| assertTrue        | VALUE                     | Assert VALUE to be *true*.                                                                                          | `assertTrue(true);`                                              |
| assertFalse       | VALUE                     | Assert VALUE to be *false*.                                                                                         | `assertFalse(false);`                                            |
| assertNotNull     | VALUE                     | Assert VALUE not to be *nullptr*.                                                                                   | `assertNotNull(&var);`                                           |
| assertZero        | VALUE                     | Assert VALUE to be *0*, where the type of *0* will match VALUE's.                                                   | `assertZero(0.0);`                                               |
| assertException   | FUNC, TYPE                | Assert FUNC to throw an exception of TYPE.                                                                          | `assertException(throw std::logic_error(""), std::logic_error);` |
| assertNoExcept    | FUNC                      | Assert FUNC not to throw any exception.                                                                             | `assertNoExcept(int i = 0);`                                     |
| assertPerformance | FUNC, MILLIS              | Assert FUNC to run in MILLIS milliseconds at maximum. The FUNC call is not interrupted, if the time exceeds MILLIS. | `assertPerformance(call(), 5);`                                  |

### Test Modules

The idea of test modules is to bind tests to an object instead of calling describes and tests directly, or in functions. In combination with the *SCTF_DEFAULT_MAIN(...)* macro, this allows to define tests with minimal effort.

**Note:** *NAME* is just the name of the module, not a string. It is encouraged to apply a naming scheme like *test_myClass*. *FN* is a block of invacations to the *test* method you already know from the testsuites, but here it is not chainable and no template version is available. Also the context is taken from the modules name. *RUNNER* is optional if you only use the default runner.

| Syntax               | Parameters       | Description                                                                                                                          | Example                                                                                                     |
| -------------------- | ---------------- | ------------------------------------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------------------------- |
| TEST_MODULE          | NAME, FN, RUNNER | Define a test module. It is basically an object that binds a testsuite to its context.                                               | `TEST_MODULE(test_myClass,{test("myClass::function",[]{myClass mc;assertTrue(mc.function());});})`          |
| TEST_MODULE_PARALLEL | NAME, FN, RUNNER | Define a test module where all tests will run in parallel. It is basically an object that binds a parallel testsuite to its context. | `TEST_MODULE_PARALLEL(test_myClass,{test("myClass::function",[]{myClass mc;assertTrue(mc.function());});})` |

## Usage

This framework is header-only. To use it, just inlude the *sctf.hpp* header file, either from source, what requires the include path set correctly, or the single file version from releases. In order to use the parallelization capability, you need to enable OpenMP at compilation. Have a look at the build pipeline, how this can be done for different platforms. As the asserts are wrapped with macros, statements inside assert statements, that have commata itself, must be written in braces.

### Floats

As floating-point comparison relies on a so called epsilon, we use the machine epsilon by default. But this may lead into false-negative test results, as it could be too accurate. In order to use a custom epsilon, there are two ways to achieve this. First, you can provide a macro in each compilation unit, which is called `SCTF_EPSILON` with a satisfying value (like 0.000001). A compiler invocation could look like "`g++ -std=c++0x test.cpp -fopenmp -DSCTF_EPSILON=0.000001`". Or you provide it before including *sctf.hpp* like in the [example](#example) below. The second way, is to provide it as an extern variable. Therefor define `SCTF_EXTERN_EPSILON` before including *sctf.hpp* and than call the `SCTF_SET_EPSILON` macro with a satisfying value in the compilation unit (cpp file). The latter one allows more fine grained control over the required epsilon value.

### Example (functional)

```cpp
// ...
/* 1st way for custom epsilon
#define SCTF_EPSILON 0.000001
*/
/* 2nd way for custom epsilon
#define SCTF_EXTERN_EPSILON
...
*/

#include "sctf.hpp"

/*
...
SCTF_SET_EPSILON(0.001)
*/

using namespace sctf;

int main(int argc, char** argv)
{
    auto rep = createPlainTextReporter(true, true);
    int x;
    describe("test")
        ->setup([&]{ x = 10; })
        ->test("1",
               [&] {
                   assert(x+1, EQ, 11);
                   assertFalse(false);
                   assertT("hell", IN, "hello", std::string);
                   int i = 101;
                   assertNotNull(&i);
                   assertNotNull(nullptr);
               })
        ->test("2",
               [] {
                   assertEquals(0, 0);
                   assertZero(0.0);
                   assertTrue(true);
                   assertT(0, UNEQUALS, 1.0, double);
                   assert(6, IN_RANGE, (std::pair<int, int>(1, 5)));
               })
        ->test("3",
               [] {
                   assert('w', IN, std::string("world"));
                   assertInInterval(2, 1, 3);
                   assert(std::string(""), IN, std::vector<std::string>{""});
                   assert(1, IN, (util::Interval<int>{1, 2}));
               })
        ->test("4", [] {
            assertException(throw std::logic_error(""), std::logic_error);
        });
    return rep->report();
}
```

### Example (OO)

```cpp

#include "sctf.hpp"

class myClass {
    bool function() {return true;}
};

TEST_MODULE(test_myClass, {
    test("myClass::function", [] {
        myClass my;
        assertTrue(my.function());
    });
    test("some other test", [] {
        assertEquals(1, 1);
    });
})

SCTF_DEFAULT_MAIN(createPlainTextReporter(true, true))
```

## Extending

### of reporters

To add a new reporter just implement the [abstract_reporter](include/reporter/abstract_reporter.hpp) interface. Have a look at the preimplemented reporters, how this is exactly done.

### of comparators

To add a new comparator you basically just need to invoke two macros.
For example:

```c
COMPARATOR(pred, "to be predecessor of", value + 1 == expect)
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
    Comparison equals<Custom>(const Custom& value, const Custom& expect)
    {
        return value.hash() == expect.hash()
               ? success
               : Comparison(equals_comp_str, to_string(value),
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

I have implemented this framework, intentionally, to test my own C++ projects, with an fundamental and extendable API. Nevertheless anybody, finding this framework useful, may use, or even extend and contribute to it.
