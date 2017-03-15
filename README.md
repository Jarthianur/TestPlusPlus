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

To add any comparator, or reporter You like, just implement the respective interface and add the factory method to the particular header-file. Like the EQUALS comparator, or the serialze method, templated functions may be specialized to handle specific types their own way.

## Usage example

```c++
#include <iostream>
#include "framework.h"
using namespace testsuite;
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
    dummy d;
    test("testsuite 1", rep)
        ->assert("static func", static_func, 0, comparator::EQUALS<int>())
        ->assert("static_f", static_f, 2, comparator::EQUALS<int>(), 1, 1);
    TestSuite_shared ts2 =
            test("testsuite 2", rep)
                ->assert("memf", &dummy::memf, d, 0, comparator::EQUALS<int>())
                ->assert("member_func", &dummy::member_func, d, 2,
                        comparator::EQUALS<int>(), 1, 1);
    ts2->assertPerformance("static_f", static_f, (std::uint64_t) 5, 2, 2)
       ->assertPerformance("memf", &dummy::memf, d, (std::uint64_t) 5);
    return rep->report();
}
```

### TODO

+ assert exceptions

#### Footnote

I implemented this framework, intentionally, to test my own C++ projects, with an fundamental and extendable API. Nevertheless anybody, finding this framework useful, may use, or even extend and contribute to it.
