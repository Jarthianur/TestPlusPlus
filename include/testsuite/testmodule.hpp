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

#include "common/types.hpp"
#include "testsuite/runner.hpp"

namespace sctf
{
namespace _
{
/**
 * Base class for test modules.
 */
// template<char const* N>
class test_module
{
protected:
    virtual ~test_module() noexcept = default;

    test_module() : _m_ts_(sctf::testsuite::create(""))
    {
        sctf::runner::instance().add_testsuite(_m_ts_);
    }

    testsuite_ptr _m_ts_;
};
}  // namespace _
}  // namespace sctf

#define SUITE(N)                                                                   \
    namespace _ns_##N                                                              \
    {                                                                              \
        class N;                                                                   \
        static const auto& _mod_              = sctf::_::singleton<N>::instance(); \
        using _type_                          = N;                                 \
        static constexpr char const _class_[] = #N;                                \
    }                                                                              \
    class _ns_##N::N : public sctf::_::test_module
//<_ns_##N::_class_>

#define TEST(N)                                                          \
    class _t_##N                                                         \
    {                                                                    \
    public:                                                              \
        _t_##N()                                                         \
        {                                                                \
            _mod_._m_ts_->test(#N, std::bind(&_type_::_tf_##N, &_mod_)); \
        }                                                                \
    } _ti_##N;                                                           \
    void _tf_##N() const

#endif  // SCTF_TESTSUITE_TESTMODULE_HPP
