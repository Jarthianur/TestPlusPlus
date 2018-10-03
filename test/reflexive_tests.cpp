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
#include <stdexcept>
#include "basic_tests.h"

using namespace sctf;

void test()
{
    auto rep = createPlainTextReporter();
    test::TestSuitesRunner runner;
    auto ts = describeParallel("ts", runner);
    ts->test("1", [] { assertTrue(true); })->test("2", [] {
        assertException(throw 1, std::logic_error);
    });
    runner.run();
    rep->report(runner);
    ts->test("3", [] { assertFalse(false); });
    rep->report(runner);
}
