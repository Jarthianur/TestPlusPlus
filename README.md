# simple-cpp-test-framework

[![codecov](https://codecov.io/gh/Jarthianur/simple-cpp-test-framework/branch/testing/graph/badge.svg)](https://codecov.io/gh/Jarthianur/simple-cpp-test-framework)

[![BCH compliance](https://bettercodehub.com/edge/badge/Jarthianur/simple-cpp-test-framework?branch=testing)](https://bettercodehub.com/)

A simple C++11, plain STL, header-only testing framework.
Featuring great **extendability** and an **easy, but powerful**, **less typing** optimized API.
**Test reports** are generated in a specified format, according to the chosen reporter.
Additionally it serves the capability to **parallelize** testruns, using *OpenMP*.

*So why actually writing a new unit-testing framework?* Except for the reason of learning how to do and self-development in C++, there is something I don't really like about many other ones. Excessive use of macros and unflexible testing methods. Yes this framework utilizes macros too, but they are neither required, nor part of the program logic. They just reduce the workload of typing and wrap functions in a usefull way.
Even test code should be regular code and also look like that. The next point is extendability. You will see later how easy it is to define new, or modify existing logic of this framework, to adjust it to your needs. There is no need to provide an ultra large allmighty framework, as some basic stuff is commonly enough and every project specific logic can be added easily.

## Contents

+ [Concept](#concept-of-simplicity)
+ [Parallelization](#some-words-about-parallelization)
+ [Feature Set](#feature-set)
+ [Usage](#usage)
+ [Example](#example)
+ [Extending](#extending)
+ [Contributing](#contributing)
+ [Footnote](#footnote)

## Concept of Simplicity

The great extendability comes from its simple structure. Also wrapping is done using macros.
Yes usually macros are evil, but in this case they are more usefull and reduce writing overhead a lot.
To gefine a new generic comparator two lines of code are necessary, one macro call for defining the comparator and one for providing a shortwrite.
As many projects define their own types and classes, which also need to be tested, one can easily specialize any comparator template to fit to their needs.

Want the reports in a custom format? So then implement the *AbstractReporter* interface.
Extensive testing is hard enough, so one should not struggle with too complex frameworks. As well as customization may be the key to make a simple framework a powerful framework. Hence the structure is kept that simple, to make it highly customizable and extendable.

## Some Words About Parallelization

This testing framework serves the capability of parallelizing tests, using OpenMP.  
It may reduce test durations massively. Nevertheless this feature should be used carefully.
That means tests, running in parallel, *must* be completely independent from each other. But this fact may also be used to test components' threadsafety.
Also consider, spawning threads has some overhead. Thus there is no point in running just a few, anyway fast, tests in parallel.  
Of course, when executed in parallel, a test suites total time is the max time of all threads.

## Feature Set

### Reporters

**Note:** Every reporter has its factory method, which is always like `createNAME(...)`, where *NAME* is the reporter class name.

| Reporter          | Description                                                                                                         |
| ----------------- | ------------------------------------------------------------------------------------------------------------------- |
| PlainTextReporter | Produces plain text, which is aimed for human-readable console output. Optionally ANSI colors can be enabled.       |
| XmlReporter       | Produces JUnit like XML format. The result may be published to any visualizer, e.g. in a jenkins environment.       |
| HtmlReporter      | Produces a single HTML file, which is a minimal standalone website showing the test results more or less beautiful. |

### Testsuite Executions

**Note:** The argument *runner* of all *describe* functions is always the *TestSuitesRunner* where to register the *TestSuite*. The argument *t_func* of all *test*,*setup*,*before*,*after* methods is always a void function without arguments, preferably a lambda expression. Calls to those methods are chainable. It is possible to add tests after the runner has run, thus run tests partially, and run it again. Already executed tests will be skipped.

| Call                | Arguments             | Description                                                                                                                                          | Example                                               |
| ------------------- | --------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------- |
| describe            | name, runner, context | Describe a testsuite with *name* and *context*, where context defaults to an empty string.                                                           | `describe("ts1", runner, "Component")...`             |
| describe<T>         | name, runner          | Describe a testsuite with *name*. The context is taken as the class-/typename of T.                                                                  | `describe<Component>("ts2", runner)...`               |
| describeParallel    | name, runner, context | Describe a testsuite with *name* and *context*, where context defaults to an empty string. Tests inside this testsuite will be executed in parallel. | `describeParallel("ts3", runner, "Component")...`     |
| describeParallel<T> | name, runner          | Describe a testsuite with *name*. The context is taken as the class-/typename of T. Tests inside this testsuite will be executed in parallel.        | `describeParallel<Component>("ts2", runner)...`       |
| test                | name, context, t_func | Create a testcase on a testsuite with *name* and *context*.                                                                                          | `...->test("test1", "Component::func", [](){...})...` |
| test                | name, t_func          | Create a testcase on a testsuite with *name*, the context is inherited from the testsuite.                                                           | `...->test("test2", [](){...})...`                    |
| test<T>             | name, t_func          | Create a testcase on a testsuite with *name*. The context is taken as the class-/typename of T.                                                      | `...->test<Component>("test3", [](){...})...`         |
| setup               | t_func                | Set a *setup* function, which will be executed once before all testcases. Exceptions thrown by the given function will get ignored.                  | `...->setup([&]{ x = 10; ... })...`                   |
| before              | t_func                | Set a *pre-test* function, which will be executed before each testcase. Exceptions thrown by the given function will get ignored.                    | `...->before([&]{ x = 10; ... })...`                  |
| after               | t_func                | Set a *post-test* function, which will be executed after each testcase. Exceptions thrown by the given function will get ignored.                    | `...->after([&]{ x = 10; ... })...`                   |

### Comparators

**Note:** While the *Comparator* names the function itself, the *Shortwrite* is what you actually write in code.

| Comparator   | Shortwrite(s) | Description                                                                                                                                                                                                                                                                                                                                  |
| ------------ | ------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| equals       | EQUALS, EQ    | Compare two values to be equal, using `operator ==`.                                                                                                                                                                                                                                                                                         |
| unequals     | UNEQUALS, NE  | Compare two values not to be equal, using `operator !=`.                                                                                                                                                                                                                                                                                     |
| greater_than | GREATER, GT   | Compare one value to be greater than another, using `operator >`.                                                                                                                                                                                                                                                                            |
| less_than    | LESS, LT      | Compare one value to be less than another, using `operator <`.                                                                                                                                                                                                                                                                               |
| in_range     | IN_RANGE, IN  | Check a value to be in range of any container. This means for containers to contain the value, for strings to contain a substring and for pairs to have it as one of the values. For the utility type [Interval](src/util/Interval.hpp), given lower and upper bounds, a value is then checked to be in this interval, including the bounds. |

### Assertions

**Note:** VALUE means the actual value you want to check. EXPECT means the expected value. COMP means the comparator. TYPE means a certain type, or class. FUNC means a function call, or instruction and is wrapped in a lambda with captions as reference. Hence FUNC can be a single instruction, or multiple split by `;`.

| Assertion         | Parameters                | Description                                                                                                               | Example                                                          |
| ----------------- | ------------------------- | ------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| assert            | VALUE, COMP, EXPECT       | Common assert. Assert successfull comparison of VALUE and EXPECT with COMP.                                               | `assert(1, LT, 1);`                                              |
| assertT           | VALUE, COMP, EXPECT, TYPE | Same as *assert*, but parameters are specialized to TYPE, which allows implicit conversions.                              | `assertT("hell", IN, "hello", std::string);`                     |
| assertEquals      | VALUE, EXPECT             | Wrapper for *assert* using *EQUALS* comparator.                                                                           | `assertEquals(1, 1);`                                            |
| assertInInterval  | VALUE, LOWER, UPPER       | Wrapper for *assert* using *IN_RANGE* and an Interval with LOWER and UPPER as bounds. Check VALUE to be in this interval. | `assertInInterval(2, 1, 3);`                                     |
| assertTrue        | VALUE                     | Assert VALUE to be *true*.                                                                                                | `assertTrue(true);`                                              |
| assertFalse       | VALUE                     | Assert VALUE to be *false*.                                                                                               | `assertFalse(false);`                                            |
| assertNotNull     | VALUE                     | Assert VALUE not to be *nullptr*.                                                                                         | `assertNotNull(&var);`                                           |
| assertZero        | VALUE                     | Assert VALUE to be *0*, where the type of *0* will match VALUE's.                                                         | `assertZero(0.0);`                                               |
| assertException   | FUNC, TYPE                | Assert FUNC to throw an exception of TYPE.                                                                                | `assertException(throw std::logic_error(""), std::logic_error);` |
| assertNoExcept    | FUNC                      | Assert FUNC not to throw any exception.                                                                                   | `assertNoExcept(int i = 0);`                                     |
| assertPerformance | FUNC, MILLIS              | Assert FUNC to run in MILLIS milliseconds at maximum. The FUNC call is not interrupted, if the time exceeds MILLIS.       | `assertPerformance(call(), 5);`                                  |


## Usage

This framework is header-only. To use it, just inlude the *sctf.hpp* header file.  
In order to use the parallelization capability compile and link the test code with *'-fopenmp'* flag. As the asserts are wrapped with macros, statements inside assert statements, that have commata itself, must be written in braces.
As floating-point comparison relies on an, so called, epsilon, we use the machine epsilon by default. But this may lead into false-negative test results, as it could be too accurate. In order to provide a custom epsilon, provide a macro in each compilation unit, which is called `SCTF_CUSTOM_EPSILON` and set it to a satisfying value (like 0.000001).
A compiler invocation could look like "`g++ -std=c++0x test.cpp -fopenmp -DSCTF_CUSTOM_EPSILON=0.000001`".

### Example

```cpp
// ...
#include "sctf.hpp"

using namespace sctf;

int main(int argc, char** argv)
{
    test::TestSuitesRunner runner;
    auto rep = createPlainTextReporter(true);
    int x;
    describe("test", runner)
        ->setup([&]{ x = 10; })
        ->test("1",
               [&]() {
                   assert(x+1, EQ, 11);
                   assertFalse(false);
                   assertT("hell", IN, "hello", std::string);
                   int i = 101;
                   assertNotNull(&i);
                   assertNotNull(nullptr);
               })
        ->test("2",
               []() {
                   assertEquals(0, 0);
                   assertZero(0.0);
                   assertTrue(true);
                   assertT(0, UNEQUALS, 1.0, double);
                   assert(6, IN_RANGE, (std::pair<int, int>(1, 5)));
               })
        ->test("3",
               []() {
                   assert('w', IN, std::string("world"));
                   assertInInterval(2, 1, 3);
                   assert(std::string(""), IN, std::vector<std::string>{""});
                   assert(1, IN, (util::Interval<int>{1, 2}));
               })
        ->test("4", []() {
            assertException(throw std::logic_error(""), std::logic_error);
        });
    return rep->report(runner);
}
```

## Extending

### Reporters

To add a new reporter just implement the [AbstractReporter](src/reporter/AbstractReporter.hpp) interface. Therefor create a class in *sctf::rep* namespace and inherit from *AbstractReporter*. Have a look at the preimplemented reporters, how this is exactly done.

### Comparators

To add a new comparator you basically just need to invoke two macros.
For example:

```c
COMPARATOR(pred, "to be predecessor of", value + 1 == expect)
PROVIDE_COMPARATOR(pred, PRE)
```

This will create a generic comparator function for you. The syntax is as follows:
`COMPARATOR(NAME, CONSTRAINT, PREDICATE)`, where *NAME* is the function name. *CONSTRAINT* is a string representing what it does and is used for reporting. *PREDICATE* is the actual comparison.
If you need some more complex comparators, have a look at the [in_range](src/comparator/inrange.hpp) to see how it is implemented.

To specialize a comparator for custom types, which do not provide a respective operator, just specialize the comparator template. For example:

```cpp
namespace sctf {
namespace comp {
    template<>
    Comparison equals<Custom>(const Custom& value, const Custom& expect)
    {
        return value.hash() == expect.hash()
               ? success
               : Comparison(equals_comp_str, util::serialize(value),
                            util::serialize(expect));
    }
}
}
```

### Serialization

To allow proper reporting, every asserted value/type needs to be serialized, or stringified somehow. If you need to serialize custom types in a certain way, just specialize the [serialize](src/util/serialize.hpp) template. For example:

```cpp
namespace sctf
{
namespace util
{
    template<>
    inline std::string serialize<Custom>(const Custom& arg)
    {
        return arg.get_id();
    }
}
}
```

## Contributing

Contribution to this project is always welcome. To keep the framework clean, we have branches for different purposes.
Generally the *master* branch is the release branch, which consists only of the framework sources and some necessary files, like the license etc. and is always coherent to the latest release.
The *release-candidate* branch contains the newest framework code, waiting for to be released.
The *testing* branch is not only the development branch, but also contains the actual tests for this framework and a CI setup.
The *gh-pages* branch is used for the GitHub page and contains examples, documentation etc.

### Workflow

Whenever a change is made to the code, these change must be made against the *testing* branch. If and only if those changes pass through the CI run, a merge to *testing* and *release-candidate* is allowed. When merging to *master* or *release-candidate* it must be assured that only changes to the actual framework are applied, neither tests, nor CI files, nor docs - with some exceptions - should appear there.
Altough this looks kinda complicated, the basic idea behind it is to sepparate contents.
So *testing* is basically a copy of *release-candidate*, but contains tests and CI setup. Hence they should always correlate to each other.

So here is the basic workflow.

+ Checkout a new branch based on current *testing*
+ Provide your changes
+ Create a PR from your branch to *testing*
+ Iff the status is green and the PR is approved, do merge and delete your branch
+ Create a PR from *testing* to *release-candidate*
+ Assure only changes to the actual framework code or README is applied to *release-candidate*
+ Iff this is assured and approved, do merge and keep *testing*
+ Tag and push from *release-candidate* a new version (rc) accordingly
+ Create a PR from *release-candidate* to *master*
+ Iff the rc is approved, do merge and keep *release-candidate*
+ Tag and push from *master* a new release version accordingly
+ Adjust the docs in *gh-pages* if necessary

#### Footnote

I (Jarthianur) have implemented this framework, intentionally, to test my own C++ projects, with an fundamental and extendable API. Nevertheless anybody, finding this framework useful, may use, or even extend and contribute to it.
