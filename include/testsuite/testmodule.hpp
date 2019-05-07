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

#ifndef SCTF_TESTSUITE_TESTMODULE_HPP_
#define SCTF_TESTSUITE_TESTMODULE_HPP_

#include "common/types.h"
#include "testsuite/runner.hpp"

#define TEST_MODULE(NAME, FN)                                 \
    namespace sctf                                            \
    {                                                         \
    class NAME : public _::test_module                        \
    {                                                         \
    public:                                                   \
        NAME() : _::test_module(), m_ctx(#NAME)               \
        {                                                     \
            runner::default_instance().register_module(this); \
        }                                                     \
        ~NAME() noexcept override = default;                  \
        void setup() override                                 \
        {                                                     \
            FN;                                               \
        }                                                     \
    };                                                        \
    static auto& s_##NAME = _::singleton<NAME>::instance();   \
    }

#endif  // SCTF_TESTSUITE_TESTMODULE_HPP_
