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

#ifndef SCTF_SCTF_HPP
#define SCTF_SCTF_HPP

#define SCFT_VERSION "1.2-rc2"

#include "comparator/equality.hpp"
#include "comparator/ordering.hpp"
#include "comparator/range.hpp"
#include "comparator/regex.hpp"
#include "reporter/console_reporter.hpp"
#include "reporter/markdown_reporter.hpp"
#include "reporter/xml_reporter.hpp"

#include "api.hpp"
#include "assert.hpp"
#include "regex.hpp"
#include "runner.hpp"

/**
 * Define a default main function, which executes and reports all tests registered to the
 * default runner.
 * @param REPORTER The reporter creation function invocation
 * @return the number of failed and erroneous tests
 */
#define SCTF_DEFAULT_MAIN(REPORTER)                        \
    int main(int /*argc*/, char** /*argv*/) {              \
        return static_cast<int>(sctf::REPORTER->report()); \
    }

#endif  // SCTF_SCTF_HPP
