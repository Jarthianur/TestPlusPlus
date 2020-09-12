/*
    Copyright (C) 2017 Jarthianur

    This file is part of simple-cpp-test-framework.

    simple-cpp-test-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple-cpp-test-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple-cpp-test-framework.  If not, see <https://www.gnu.org/licenses/>.
*/

/// @file

#ifndef SCTF_SCTF_HPP
#define SCTF_SCTF_HPP

/// SCTF release version this source belongs to.
#define SCFT_VERSION "2.1-rc1"

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
 * Define a default main function, which performs all tests, and produces the report with specified
 * reporter.
 *
 * @param R is the reporters factory method invokation.
 */
#define SCTF_DEFAULT_MAIN(R)                                      \
    auto main(int /*argc*/, char* * /*argv*/)->int {              \
        return static_cast<int>(sctf::runner::instance().run(R)); \
    }

#endif  // SCTF_SCTF_HPP
