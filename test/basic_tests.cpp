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

#include "basic_tests.h"

#include <sstream>

#include "traits.hpp"

using namespace sctf;

void test_traits();

void basic_tests()
{
    test_traits();
}

void test_traits()
{
    // is_streamable
    throw_if_not_streamable<std::ostringstream, streamable>();
    try
    {
        throw_if_not_streamable<std::ostringstream, void_type>();
        throw Failure("Given type should be not streamable");
    }
    catch(const std::logic_error&)
    {
    }
    try
    {
        throw_if_not_streamable<std::ostringstream, not_streamable>();
        throw Failure("Given type should be not streamable");
    }
    catch(const std::logic_error&)
    {
    }

    // is_iterable
    throw_if_not_iterable<iterable>();
    try
    {
        throw_if_not_iterable<void_type>();
        throw Failure("Given type should be not iterable");
    }
    catch(const std::logic_error&)
    {
    }
    try
    {
        throw_if_not_iterable<not_iterable>();
        throw Failure("Given type should be not iterable");
    }
    catch(const std::logic_error&)
    {
    }

    // is_ordinal
    throw_if_not_ordinal<ordinal>();
    try
    {
        throw_if_not_ordinal<void_type>();
        throw Failure("Given type should be not ordinal");
    }
    catch(const std::logic_error&)
    {
    }
    try
    {
        throw_if_not_ordinal<not_ordinal>();
        throw Failure("Given type should be not ordinal");
    }
    catch(const std::logic_error&)
    {
    }

    // is_equal_comparable
    throw_if_not_equal_comparable<equal_comparable>();
    try
    {
        throw_if_not_equal_comparable<void_type>();
        throw Failure("Given type should be not equal-comparable");
    }
    catch(const std::logic_error&)
    {
    }
    try
    {
        throw_if_not_equal_comparable<not_equal_comparable>();
        throw Failure("Given type should be not equal-comparable");
    }
    catch(const std::logic_error&)
    {
    }

    // is_unequal_comparable
    throw_if_not_unequal_comparable<unequal_comparable>();
    try
    {
        throw_if_not_unequal_comparable<void_type>();
        throw Failure("Given type should be not unequal-comparable");
    }
    catch(const std::logic_error&)
    {
    }
    try
    {
        throw_if_not_unequal_comparable<not_unequal_comparable>();
        throw Failure("Given type should be not unequal-comparable");
    }
    catch(const std::logic_error&)
    {
    }
}
