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

#include <iostream>

#include "basic_tests.h"
#include "reflexive_tests.h"
#include "sctf.hpp"

using namespace sctf;

int main(int, char**)
{
    runner runner;
    auto   rep = createPlainTextReporter(true, true);

    try
    {
        basic_tests();
    }
    catch (const std::exception& e)
    {
        std::cout << "Basic tests have failed! [" << e.what() << "]" << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cout << "Basic tests have failed!" << std::endl;
        return 1;
    }
    std::cout << "Basic tests have succeeded!" << std::endl;

    reflexive_tests(runner);

    return rep->report(runner) > 0 ? 1 : 0;
}
