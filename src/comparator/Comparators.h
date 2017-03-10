#ifndef COMPARATOR_COMPARATORS_H_
#define COMPARATOR_COMPARATORS_H_

#include "ComparatorStrategy.hpp"
#include "Equals.hpp"
#include <memory>

namespace comparator {

using Comparator = std::shared_ptr<ComparatorStrategy>;

inline Comparator EQUALS()
{
    return Comparator(new Equals());
}

}

#endif /* COMPARATOR_COMPARATORS_H_ */
