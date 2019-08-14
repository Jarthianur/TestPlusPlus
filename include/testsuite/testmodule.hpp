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

#include "common/types.hpp"
#include "testsuite/runner.hpp"

namespace sctf
{
namespace _
{
/**
 * @brief Base class for test modules.
 */
class test_module
{
protected:
    test_module(const testsuite_shared& ts) : m_ts(ts) {}
    virtual ~test_module() noexcept = default;

    inline void test(const char* name, _::test_function&& fn)
    {
        m_ts->test(name, std::move(fn));
    }

    inline void setup(_::test_function&& fn)
    {
        m_ts->setup(std::move(fn));
    }

    inline void before(_::test_function&& fn)
    {
        m_ts->before(std::move(fn));
    }

    inline void after(_::test_function&& fn)
    {
        m_ts->after(std::move(fn));
    }

    testsuite_shared m_ts;
};
}  // namespace _
}  // namespace sctf

/**
 * @def TEST_MODULE(NAME, FN, ...)
 * @brief Define a test module for OO testing approach.
 * Invoke this macro to create and instantiate a test module. The resulting module is bound to a
 * single testsuite.
 * @param NAME The name of this module (not a string!), preferably like test_myClass
 * @param FN The test function body, which is a block of calls to `test(...)`
 * @param ... Allows to optionally pass a runner
 */
#define TEST_MODULE(NAME, FN, ...)                                                 \
    class NAME : public sctf::_::test_module                                       \
    {                                                                              \
    public:                                                                        \
        NAME() : sctf::_::test_module(sctf::describe(#NAME, #NAME, ##__VA_ARGS__)) \
        {                                                                          \
            FN;                                                                    \
        }                                                                          \
        ~NAME() noexcept override = default;                                       \
    };                                                                             \
    namespace sctf                                                                 \
    {                                                                              \
    namespace _                                                                    \
    {                                                                              \
    static const auto& mods_##NAME = singleton<NAME>::instance();                  \
    }                                                                              \
    }

/**
 * @def TEST_MODULE_PARALLEL(NAME, FN, ...)
 * @brief Define a test module for OO testing approach. Tests will run in parallel.
 * Invoke this macro to create and instantiate a test module. The resulting module is bound to a
 * single testsuite.
 * @param NAME The name of this module (not a string!), preferably like test_myClass
 * @param FN The test function body, which is a block of calls to `test(...)`
 * @param ... Allows to optionally pass a runner
 */
#define TEST_MODULE_PARALLEL(NAME, FN, ...)                                                \
    class NAME : public sctf::_::test_module                                               \
    {                                                                                      \
    public:                                                                                \
        NAME() : sctf::_::test_module(sctf::describeParallel(#NAME, #NAME, ##__VA_ARGS__)) \
        {                                                                                  \
            FN;                                                                            \
        }                                                                                  \
        ~NAME() noexcept override = default;                                               \
    };                                                                                     \
    namespace sctf                                                                         \
    {                                                                                      \
    namespace _                                                                            \
    {                                                                                      \
    static const auto& modp_##NAME = singleton<NAME>::instance();                          \
    }                                                                                      \
    }

#endif  // SCTF_TESTSUITE_TESTMODULE_HPP
