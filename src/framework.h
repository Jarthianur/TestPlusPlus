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

#ifndef SRC_FRAMEWORK_H_
#define SRC_FRAMEWORK_H_

#include "comparator/Equals.hpp"
#include "comparator/Greater.hpp"
#include "comparator/Less.hpp"
#include "comparator/Unequals.hpp"
#include "reporter/ColoredReporter.hpp"
#include "reporter/HtmlReporter.hpp"
#include "reporter/PlainTextReporter.hpp"
#include "reporter/XmlReporter.hpp"
#include "testsuite/TestSuite.hpp"
#include "util/assert.hpp"
#include "util/types.h"

#define PROVIDE_COMPARATOR(TYPE, COMP, NAME) static Comparator<TYPE> NAME = COMP<TYPE>();

#endif /* SRC_FRAMEWORK_H_ */
