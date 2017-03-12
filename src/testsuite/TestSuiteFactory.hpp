/*
 Copyright_License {

 Copyright (C) 2017 Julian P. Becht
 Author: Julian P. Becht

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License version 3
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 }
 */

#ifndef TESTSUITE_TESTSUITEFACTORY_HPP_
#define TESTSUITE_TESTSUITEFACTORY_HPP_

#include <memory>
#include <string>

#include "../reporter/AbstractReporter.hpp"
#include "TestSuite.hpp"
#include "TestSuite_shared.h"

namespace testsuite
{

/**
 * Factory method to create a TestSuite with given reporter.
 * test: representative name/description
 */
inline TestSuite_shared test(const std::string& test,
                             std::shared_ptr<reporter::AbstractReporter> reporter)
{
    TestSuite_shared ts = TestSuite::create(test);
    reporter->registerTestSuite(ts);
    return ts;
}

} // testsuite

#endif /* TESTSUITE_TESTSUITEFACTORY_HPP_ */
