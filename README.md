# simple-cpp-test-framework

A simple C++11, plain STL, header-only Testsuite.  
Featuring great extendability and an easy, but powerful less-typing optimized API.
Test reports are generated in a specified format, according to the chosen reporter.

Currently preimplemented:

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
  + assert exception
+ parallel execution with openmp

To add any comparator, or reporter You like, just implement the respective interface and add the factory method to the particular header-file. Like the EQUALS comparator, or the serialze method, templated functions may be specialized to handle specific types their own way. To enable parallel execution support, compile and link with `-fopenmp`.

## Usage example

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
        ->test("another testcase", "dummy", [](){
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
