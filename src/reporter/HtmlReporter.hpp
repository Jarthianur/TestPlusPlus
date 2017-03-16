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

#ifndef SRC_REPORTER_HTMLREPORTER_HPP_
#define SRC_REPORTER_HTMLREPORTER_HPP_

#include <cstdint>
#include <iostream>
#include <string>

#include "../testsuite/TestCase.hpp"
#include "../testsuite/TestStats.hpp"
#include "../testsuite/TestSuite.hpp"
#include "../util/types.h"
#include "AbstractReporter.hpp"

namespace testsuite
{
namespace reporter
{

/**
 * Concrete reporter,
 * featuring full html website format.
 */
class HtmlReporter: public AbstractReporter
{
public:
    /**
     * c'tor with stream
     */
    inline HtmlReporter(std::ostream& stream)
            : AbstractReporter(stream)
    {
    }

    /**
     * d'tor
     */
    inline virtual ~HtmlReporter() noexcept
    {
    }

protected:
    /**
     * impl
     */
    inline virtual void reportTestSuite(TestSuite_shared ts)
    {
        AbstractReporter::reportTestSuite(ts);
    }

    /**
     * impl
     */
    virtual void reportTestCase(TestCase& tc)
    {

    }

    /**
     * impl
     */
    virtual void beginReport()
    {
    }

    /**
     * impl
     */
    virtual void endReport()
    {

    }

private:

};

} // reporter
} // testsuite

#endif /* SRC_REPORTER_HTMLREPORTER_HPP_ */
