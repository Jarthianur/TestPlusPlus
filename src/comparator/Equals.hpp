#ifndef COMPARATOR_EQUALS_HPP_
#define COMPARATOR_EQUALS_HPP_

#include "ComparatorStrategy.hpp"

namespace comparator
{

class Equals: public ComparatorStrategy
{
public:
    inline Equals()
            : ComparatorStrategy()
    {
    }
    inline virtual ~Equals() throw ()
    {
    }
    inline bool compare(int a, int b)
    {
        return a == b;
    }
    inline bool compare(unsigned int a, unsigned int b)
    {
        return a == b;
    }
    inline bool compare(double a, double b)
    {
        return a == b;
    }
    inline bool compare(bool a, bool b)
    {
        return a == b;
    }
    inline bool compare(const std::string& a, const std::string& b)
    {
        return a == b;
    }
    inline bool compare(const char* a, const char* b)
    {
        return std::string(a) == std::string(b);
    }
    inline bool compare(char a, char b)
    {
        return a == b;
    }
};

}

#endif /* COMPARATOR_EQUALS_HPP_ */
