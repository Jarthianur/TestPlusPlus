#include <iostream>
#include <memory>
#include <string>

#include "comparator/Comparators.h"
#include "comparator/ComparatorStrategy.hpp"
#include "reporter/AbstractReporter.hpp"
#include "testsuite/TestSuite.hpp"
#include "testsuite/TestSuiteFactory.hpp"
#include "reporter/ColoredReporter.hpp"

using namespace testsuite;

int dothat(int i)
{
    while (++i < 1000000)
    {
    }
    return i;
}

std::string doit(const char* a, const char* b)
{
    return a < b ? "less" : "more";
}

int main(int argc, char** argv)
{
    auto reporter = std::shared_ptr<AbstractReporter>(new ColoredReporter(std::cout));

    test("test", reporter)->assert("dothat", dothat, 1, comparator::EQUALS(), 1)->assert(
            "doit", doit, "more", comparator::EQUALS(), "1", "2")->assert(
            "doit", doit, "more", comparator::EQUALS(), "1", "2")->assert(
            "doit", doit, "less", comparator::EQUALS(), "1", "2")->assert(
            "doit", doit, "more", comparator::EQUALS(), "1", "2");

    reporter->report();
    return 0;
}
