# simple-cpp-testsuite

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
using namespace testsuite;
//...
int doThat(int i) { return i; }
std::string doIt(const char* a, const char* b) {
    return std::string(a).append(b);
}
//...
auto report = reporter::createXmlReporter(std::cout);

test("Any testsuite", report)
    ->assert("A first testcase", doThat, 1, comparator::EQUALS<int>(), 1)
    ->assert("A failing testcase", doIt, std::string("hello"),
             comparator::EQUALS<std::string>(), "a", "b");

test("Another testsuite", report)
    ->assert("A test with lambda", [](){ return 1; }, 1, comparator::EQUALS<int>())
    ->assertPerformance("test runtime", doThat, 6, 123);
//...
return report->report();
```

### TODO

+ assert exceptions

#### Footnote

I implemented this framework, intentionally, to test my own C++ projects, with an fundamental and extendable API. Nevertheless anybody, finding this framework useful, may use, or even extend and contribute to it.
