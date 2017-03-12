#include <iostream>
#include <memory>
#include <string>

#include "comparator/Comparators.h"
#include "reporter/Reporters.hpp"
#include "testsuite/TestSuiteFactory.hpp"

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
    auto report = std::shared_ptr<reporter::AbstractReporter>(new reporter::XmlReporter(std::cout));

    test("test", report)->assert("dothat", dothat, 1, comparator::EQUALS(), 1)->assert(
            "doit", doit, std::string("more"), comparator::EQUALS(), "1", "2")->assert(
            "doit", doit, std::string("more"), comparator::EQUALS(), "1", "2")->assert(
            "doit", doit, std::string("less"), comparator::EQUALS(), "1", "2")->assert(
            "doit", []()
            {   return 0;},
            0, comparator::EQUALS());

    return report->report();
}
