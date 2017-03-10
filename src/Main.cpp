#include <iostream>
#include <memory>

#include "comparator/Comparators.h"
#include "comparator/ComparatorStrategy.hpp"
#include "reporter/AbstractReporter.hpp"
#include "reporter/PlainTextReporter.hpp"
#include "testsuite/TestSuite.hpp"
#include "testsuite/TestSuiteFactory.hpp"

using namespace testsuite;

int main(int argc, char** argv)
{
    auto reporter = std::shared_ptr<AbstractReporter>(new PlainTextReporter(std::cout));

    test("test", reporter)->assert("dothat", 1, 1, comparator::EQUALS(), 1)->assert(
            "dothat", "value...", "expected", comparator::EQUALS(), 1)->assert(
            "dothat", "expected", "expected", comparator::EQUALS(), "someinput",
            "somemoreinput");

    reporter->report();
    return 0;
}
