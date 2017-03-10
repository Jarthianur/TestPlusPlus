#include <functional>
#include <iostream>
#include <memory>

#include "comparator/Comparators.h"
#include "comparator/ComparatorStrategy.hpp"
#include "reporter/PlainTextReporter.hpp"
#include "testsuite/TestCase.hpp"
#include "testsuite/TestStatistic.hpp"
#include "testsuite/TestSuite.hpp"

using namespace testsuite;

int main(int argc, char** argv)
{
    auto reporter = std::shared_ptr<AbstractReporter>(new PlainTextReporter(std::cout));

    test("test", std::ref(reporter))->assert("dothat", 1, 1, comparator::EQUALS(), 1)->assert(
            "dothat", "value...", "expected", comparator::EQUALS())->assert(
            "dothat", "expected", "expected", comparator::EQUALS(), "someinput",
            "somemoreinput");

    return teststatistic::num_of_fails;
}
