# simple-cpp-test-framework

A simple C++11, plain STL, header-only testing framework.  
Featuring great **extendability** and an **easy, but powerful**, **less-typing** optimized API.  
Test reports are generated in a specified format, according to the chosen reporter.
Additionally it serves the capability to **parallelize** testruns, using *OpenMP*.

## Contents

+ [Preimplemented](#currently-preimplemented)
+ [Concept](#concept-of-simplicity)
+ [Parallelization](#some-words-about-parallelization)
+ [Usage](#usage)
+ [Footnote](#footnote)

## Currently Preimplemented

+ reporters
  + a plain text reporter
  + &uarr; the same with ANSI colors
  + JUnit style XML reporter
+ comparators
  + equal to
  + less than
  + greater than
+ assertions
  + normal assert with comparator
  + performance assert (analysing method runtime)
  + assert exceptions
+ parallel execution

## Concept of Simplicity

The great extendability comes from its simple structure.  
If You need one, just implement the *Comparator* interface and add a factory method to the *Comparators.hpp*. This allows users to choose any comparator with just a few characters to type. Also everyone can add any comparator to compare any type or object, even project specific ones.  
Want the reports in a custom format? So then implement the *Reporter* interface and add a factory method to the *Reporters.hpp*. Sounds familiar, doesn't it?  
Extensive testing is hard enough, so one should not struggle with too complex frameworks.  
As well as customization may be the key to make a simple framework a powerful framework. Hence the structure is kept that simple, to make it highly customizable and extendable.

## Some Words About Parallelization

This testing framework serves the capability of parallelizing tests, using OpenMP.  
It may reduce test durations massively. Nevertheless this feature should be used carefully.
That means tests, running in parallel, *must* be completely independent from each other. But this fact may also be used to test components threadsafety.
Also consider, spawning threads has some overhead. Thus there is no point in running just a few, anyway fast, tests in parallel.  
Of course, when executed in parallel, a test suites total time is the max time of all threads.

## Usage

This framework is header-only. To use it, just inlude the *framework.h* header file.  
In order to use the parallelization capability compile and link the test code with *'-fopenmp'* flag.

### Example

```c++
#include <iostream>
#include "framework.h"
using namespace testsuite;
using namespace comparator;
// prevent name conflict in case assert is defined
#ifdef assert
#undef assert
#endif

int static_func()
{
    return 0;
}
int static_f(int i, int b)
{
    return i + b;
}
int throwal()
{
    throw std::logic_error("Hallo Welt!");
}

class dummy
{
public:
    inline dummy()
    {
    }
    inline virtual ~dummy() throw ()
    {
    }
    inline int member_func(int i, int a)
    {
        return i + a;
    }
    inline int memf()
    {
        return 0;
    }
};

int main(int argc, char** argv)
{
    auto rep = reporter::createXmlReporter(std::cout);
    TestSuitesRunner runner;

    describe("a testsuite", runner)
        ->test("a testcase", "main", [](){
            assert(static_f(1,2), 3, EQUALS<int>());
            assertException<std::logic_error>(throwal());
        })
        ->test<dummy>("another testcase", [](){
            dummy d;
            assertPerformance([](){
                d.memf();
            }, 2.0);
        });

    runner.executeAll(); //explicit call, or ..

    return rep->report(runner); // ... implicit
}
```

#### Footnote

I implemented this framework, intentionally, to test my own C++ projects, with an fundamental and extendable API. Nevertheless anybody, finding this framework useful, may use, or even extend and contribute to it.
