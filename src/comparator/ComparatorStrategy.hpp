#ifndef COMPARATOR_COMPARATORSTRATEGY_HPP_
#define COMPARATOR_COMPARATORSTRATEGY_HPP_

#include <string>

namespace testsuite
{
namespace comparator
{

class ComparatorStrategy
{
public:
    inline ComparatorStrategy(const std::string& assert)
            : assertion(assert)
    {
    }
    virtual ~ComparatorStrategy() throw ()
    {
    }
    virtual bool compare(int, int) = 0;
    virtual bool compare(double, double) = 0;
    virtual bool compare(const std::string&, const std::string&) = 0;
    virtual bool compare(unsigned int, unsigned int) = 0;
    virtual bool compare(bool, bool) = 0;
    virtual bool compare(const char*, const char*) = 0;
    virtual bool compare(char, char) = 0;
    const std::string assertion;
};

} // comparator
} // testsuite

#endif /* COMPARATOR_COMPARATORSTRATEGY_HPP_ */
