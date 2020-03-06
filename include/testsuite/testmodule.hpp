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

#ifndef SCTF_TESTSUITE_TESTMODULE_HPP
#define SCTF_TESTSUITE_TESTMODULE_HPP

#include <functional>

#include "common/stringify.hpp"
#include "common/types.hpp"
#include "testsuite/runner.hpp"

#define SCTF_PRIVATE_CONCAT3(A, B, C) A##B##C
#define SCTF_PRIVATE_TEST_NAME(ID) SCTF_PRIVATE_CONCAT3(sctf_private_test_, ID, _)
#define SCTF_PRIVATE_TEST_INST(ID) SCTF_PRIVATE_CONCAT3(sctf_private_test_, ID, _inst_)
#define SCTF_PRIVATE_TEST_FN(ID) SCTF_PRIVATE_CONCAT3(sctf_private_test_fn_, ID, _)
#define SCTF_PRIVATE_SUITE_NS(ID) SCTF_PRIVATE_CONCAT3(sctf_private_ns_, ID, _)
#define SCTF_PRIVATE_SUITE_NAME(ID) SCTF_PRIVATE_CONCAT3(sctf_private_suite_, ID, _)

#define SCTF_PRIVATE_SUITE_WRAPPER(DESCR, BASE)                                       \
    namespace SCTF_PRIVATE_SUITE_NS(__LINE__) {                                       \
        class test_module                                                             \
        {                                                                             \
        protected:                                                                    \
            virtual ~test_module() noexcept = default;                                \
            test_module() : sctf_private_m_ts_(sctf::private_::BASE::create(DESCR)) { \
                sctf::private_::runner::instance().add_testsuite(sctf_private_m_ts_); \
            }                                                                         \
            sctf::private_::testsuite_ptr sctf_private_m_ts_;                         \
        };                                                                            \
        class SCTF_PRIVATE_SUITE_NAME(__LINE__);                                      \
        static const auto& sctf_private_mod_ =                                        \
            sctf::private_::singleton<SCTF_PRIVATE_SUITE_NAME(__LINE__)>::instance(); \
        using sctf_private_mod_type_ = SCTF_PRIVATE_SUITE_NAME(__LINE__);             \
    }                                                                                 \
    class SCTF_PRIVATE_SUITE_NS(__LINE__)::SCTF_PRIVATE_SUITE_NAME(__LINE__)          \
        : public SCTF_PRIVATE_SUITE_NS(__LINE__)::test_module

#define SUITE(NAME) SCTF_PRIVATE_SUITE_WRAPPER(NAME, testsuite)
#define SUITE_PAR(NAME) SCTF_PRIVATE_SUITE_WRAPPER(NAME, testsuite_parallel)
#define DESCRIBE(NAME) SUITE(NAME)
#define DESCRIBE_PAR(NAME) SUITE_PAR(NAME)

#define TEST(NAME)                                                                               \
    class SCTF_PRIVATE_TEST_NAME(__LINE__)                                                       \
    {                                                                                            \
    public:                                                                                      \
        SCTF_PRIVATE_TEST_NAME(__LINE__)(sctf_private_mod_type_ * mod_) {                        \
            mod_->sctf_private_m_ts_->test(                                                      \
                NAME, std::bind(&sctf_private_mod_type_::SCTF_PRIVATE_TEST_FN(__LINE__), mod_)); \
        }                                                                                        \
    } SCTF_PRIVATE_TEST_INST(__LINE__){this};                                                    \
    void SCTF_PRIVATE_TEST_FN(__LINE__)()

#define IT(DESCR) TEST("It " DESCR)

#define SCTF_PRIVATE_FN_WRAPPER(FN)                                                 \
    class sctf_private_##FN##_                                                      \
    {                                                                               \
    public:                                                                         \
        sctf_private_##FN##_(sctf_private_mod_type_* mod_) {                        \
            mod_->sctf_private_m_ts_->FN(                                           \
                std::bind(&sctf_private_mod_type_::sctf_private_##FN##_fn_, mod_)); \
        }                                                                           \
    } sctf_private_##FN##_inst_{this};                                              \
    void sctf_private_##FN##_fn_()

#define BEFORE_EACH() SCTF_PRIVATE_FN_WRAPPER(before_each)
#define AFTER_EACH() SCTF_PRIVATE_FN_WRAPPER(after_each)
#define SETUP() SCTF_PRIVATE_FN_WRAPPER(setup)
#define TEARDOWN() SCTF_PRIVATE_FN_WRAPPER(teardown)

#endif  // SCTF_TESTSUITE_TESTMODULE_HPP
