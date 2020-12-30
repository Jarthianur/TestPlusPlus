/*
    Copyright (C) 2017  Jarthianur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/// @file

#ifndef TPP_API_HPP
#define TPP_API_HPP

#include <utility>

namespace tpp
{
namespace intern
{
template<typename T>
struct singleton final
{
    template<typename... Args>
    static auto
    instance(Args&&... args_) noexcept -> T const& {
        static T const inst(std::forward<Args>(args_)...);
        return inst;
    }
};
}  // namespace intern
}  // namespace tpp

#define TPP_INTERN_CONCAT3(A, B, C) A##B##C
#define TPP_INTERN_API_TEST_NAME(ID) TPP_INTERN_CONCAT3(tpp_intern_test_, ID, _)
#define TPP_INTERN_API_TEST_INST(ID) TPP_INTERN_CONCAT3(tpp_intern_test_, ID, _inst_)
#define TPP_INTERN_API_TEST_FN(ID) TPP_INTERN_CONCAT3(tpp_intern_test_fn_, ID, _)
#define TPP_INTERN_API_SUITE_NS(ID) TPP_INTERN_CONCAT3(tpp_intern_ns_, ID, _)
#define TPP_INTERN_API_SUITE_NAME(ID) TPP_INTERN_CONCAT3(tpp_intern_suite_, ID, _)

#define TPP_INTERN_API_SUITE_WRAPPER(DESCR, BASE)                                                      \
    namespace TPP_INTERN_API_SUITE_NS(__LINE__) {                                                      \
        class test_module                                                                              \
        {                                                                                              \
            tpp::intern::test::testsuite_ptr m_ts_;                                                    \
                                                                                                       \
        public:                                                                                        \
            test_module(test_module const&)     = delete;                                              \
            test_module(test_module&&) noexcept = delete;                                              \
            virtual ~test_module() noexcept     = default;                                             \
            auto                                                                                       \
            operator=(test_module const&) -> test_module& = delete;                                    \
            auto                                                                                       \
            operator=(test_module&&) noexcept -> test_module& = delete;                                \
                                                                                                       \
        protected:                                                                                     \
            test_module() : m_ts_(tpp::intern::test::BASE::create(DESCR)) {                            \
                tpp::runner::instance().add_testsuite(m_ts_);                                          \
            }                                                                                          \
            auto                                                                                       \
            tpp_intern_ts_() const -> tpp::intern::test::testsuite_ptr const& {                        \
                return m_ts_;                                                                          \
            }                                                                                          \
        };                                                                                             \
        class TPP_INTERN_API_SUITE_NAME(__LINE__);                                                     \
        using tpp_intern_mod_type_         = TPP_INTERN_API_SUITE_NAME(__LINE__);                      \
        static auto const& tpp_intern_mod_ = tpp::intern::singleton<tpp_intern_mod_type_>::instance(); \
    }                                                                                                  \
    class TPP_INTERN_API_SUITE_NS(__LINE__)::TPP_INTERN_API_SUITE_NAME(__LINE__)                       \
        : public TPP_INTERN_API_SUITE_NS(__LINE__)::test_module

#define TPP_INTERN_API_TEST_WRAPPER(DESCR)                                                          \
    class TPP_INTERN_API_TEST_NAME(__LINE__)                                                        \
    {                                                                                               \
    public:                                                                                         \
        explicit TPP_INTERN_API_TEST_NAME(__LINE__)(tpp_intern_mod_type_ * mod_) {                  \
            mod_->tpp_intern_ts_()->test(DESCR, [=] { mod_->TPP_INTERN_API_TEST_FN(__LINE__)(); }); \
        }                                                                                           \
    } TPP_INTERN_API_TEST_INST(__LINE__){this};                                                     \
    void TPP_INTERN_API_TEST_FN(__LINE__)()

#define TPP_INTERN_API_FN_WRAPPER(FN)                                           \
    class tpp_intern_##FN##_                                                    \
    {                                                                           \
    public:                                                                     \
        explicit tpp_intern_##FN##_(tpp_intern_mod_type_* mod_) {               \
            mod_->tpp_intern_ts_()->FN([=] { mod_->tpp_intern_##FN##_fn_(); }); \
        }                                                                       \
    } tpp_intern_##FN##_inst_{this};                                            \
    void tpp_intern_##FN##_fn_()

/**
 * Create a testsuite, where all testcases run sequentially.
 *
 * @param DESCR is a cstring with the description, or name of the testsuite.
 *
 * EXAMPLE:
 * @code
 * SUITE("test some stuff") {
 *   // testcases
 * };
 * @endcode
 */
#define SUITE(DESCR) TPP_INTERN_API_SUITE_WRAPPER(DESCR, testsuite)

/**
 * Create a testsuite, where all testcases are run in parallel.
 *
 * @param DESCR is a cstring with the description, or name of the testsuite.
 *
 * EXAMPLE:
 * @code
 * SUITE_PAR("test some stuff in parallel") {
 *   // testcases
 * };
 * @endcode
 */
#define SUITE_PAR(DESCR) TPP_INTERN_API_SUITE_WRAPPER(DESCR, testsuite_parallel)

/**
 * Create a testsuite, where all testcases run sequentially.
 *
 * @param DESCR is a cstring with the description, or name of the testsuite.
 *
 * EXAMPLE:
 * @code
 * DESCRIBE("test some stuff") {
 *   // testcases
 * };
 * @endcode
 */
#define DESCRIBE(DESCR) SUITE(DESCR)

/**
 * Create a testsuite, where all testcases are run in parallel.
 *
 * @param DESCR is a cstring with the description, or name of the testsuite.
 *
 * EXAMPLE:
 * @code
 * DESCRIBE_PAR("test some stuff in parallel") {
 *   // testcases
 * };
 * @endcode
 */
#define DESCRIBE_PAR(DESCR) SUITE_PAR(DESCR)

/**
 * Create a testcase.
 *
 * @param DESCR is a cstring with the description, or name of the testcase.
 *
 * EXAMPLE:
 * @code
 * TEST("some test") {
 *   // assertions
 * }
 * @endcode
 */
#define TEST(DESCR) TPP_INTERN_API_TEST_WRAPPER(DESCR)

/**
 * Create a testcase.
 *
 * @param DESCR is a cstring with the description, or name of the testcase.
 *
 * EXAMPLE:
 * @code
 * IT("should do something") {
 *   // assertions
 * }
 * @endcode
 */
#define IT(DESCR) TPP_INTERN_API_TEST_WRAPPER("It " DESCR)

/**
 * Create a definition for a function as part of a testsuite, that is executed once before each
 * testcase is run.
 *
 * EXAMPLE:
 * @code
 * BEFORE_EACH() {
 *   // do something
 * }
 * @endcode
 */
#define BEFORE_EACH() TPP_INTERN_API_FN_WRAPPER(before_each)

/**
 * Create a definition for a function as part of a testsuite, that is executed once after each
 * testcase is ran.
 *
 * EXAMPLE:
 * @code
 * AFTER_EACH() {
 *   // do something
 * }
 * @endcode
 */
#define AFTER_EACH() TPP_INTERN_API_FN_WRAPPER(after_each)

/**
 * Create a definition for a function as part of a testsuite, that is executed once before all
 * testcases are run.
 *
 * EXAMPLE:
 * @code
 * SETUP() {
 *   // do something
 * }
 * @endcode
 */
#define SETUP() TPP_INTERN_API_FN_WRAPPER(setup)

/**
 * Create a definition for a function as part of a testsuite, that is executed once after all
 * testcases are ran.
 *
 * EXAMPLE:
 * @code
 * TEARDOWN() {
 *   // do something
 * }
 * @endcode
 */
#define TEARDOWN() TPP_INTERN_API_FN_WRAPPER(teardown)

#endif  // TPP_API_HPP
