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

#ifndef REPORTER_REPORTERS_HPP_
#define REPORTER_REPORTERS_HPP_

#include <iostream>
#include <memory>

#include "AbstractReporter.hpp"
#include "ColoredReporter.hpp"
#include "PlainTextReporter.hpp"
#include "XmlReporter.hpp"

namespace testsuite
{
namespace reporter
{

/**
 * Factory for reporters.
 * Define factory method for each reporter in here.
 */

/**
 * Factory method for plain text reporter.
 */
inline AbstractReporter_shared createPlainTextReporter(std::ostream& stream)
{
    return AbstractReporter_shared(new PlainTextReporter(stream));
}

/**
 * Factory method for colored text reporter
 */
inline AbstractReporter_shared createColoredReporter(std::ostream& stream)
{
    return AbstractReporter_shared(new ColoredReporter(stream));
}

/**
 * Factory method for xml reporter
 */
inline AbstractReporter_shared createXmlReporter(std::ostream& stream)
{
    return AbstractReporter_shared(new XmlReporter(stream));
}

} // reporter
} // testsuite

#endif /* REPORTER_REPORTERS_HPP_ */
