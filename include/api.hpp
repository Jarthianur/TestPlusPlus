/*
    Copyright (C) 2017 Jarthianur

    This file is part of TestPlusPlus (Test++).

    TestPlusPlus (Test++) is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TestPlusPlus (Test++) is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TestPlusPlus (Test++).  If not, see <https://www.gnu.org/licenses/>.
*/

/// @file

#ifndef TPP_API_HPP
#define TPP_API_HPP

#include <utility>

namespace tpp
{
namespace intern
{
/**
 * Provide a singleton pattern implementation that mitigates static initialization order fiasko.
 * The instance exists in global scope.
 */
template<typename T>
struct singleton final
{
    /**
     * Get the underlying instance helt by this singleton.
     * The underlying instance is constructed once, on first call.
     * Further calls to this singleton return the same instance.
     *
     * @tparam Args
     * An unspecified generic list of arguments, which is forwarded to the constructor
     * at first call.
     * @return the underlying instance.
     */
    template<typename... Args>
    static auto
    instance(Args&&... args_) noexcept -> T const& {
        static T const inst(std::forward<Args>(args_)...);
        return inst;
    }
};
}  // namespace intern
}  // namespace tpp

/**
 * Concatenate three symbols.
 *
 * @param A is the left symbol.
 * @param B is the middle symbol.
 * @param C is the right symbol.
 */
#define TPP_INTERN_CONCAT3(A, B, C) A##B##C

/**
 * Produce an identifier for a testcase name.
 *
 * @param ID is the unique identifier part.
 */
#define TPP_INTERN_API_TEST_NAME(ID) TPP_INTERN_CONCAT3(tpp_intern_test_, ID, _)

/**
 * Produce an identifier for a testcase instance.
 *
 * @param ID is the unique identifier part.
 */
#define TPP_INTERN_API_TEST_INST(ID) TPP_INTERN_CONCAT3(tpp_intern_test_, ID, _inst_)

/**
 * Produce an identifier for a testcase function.
 *
 * @param ID is the unique identifier part.
 */
#define TPP_INTERN_API_TEST_FN(ID) TPP_INTERN_CONCAT3(tpp_intern_test_fn_, ID, _)

/**
 * Produce an identifier for a testsuite namespace.
 *
 * @param ID is the unique identifier part.
 */
#define TPP_INTERN_API_SUITE_NS(ID) TPP_INTERN_CONCAT3(tpp_intern_ns_, ID, _)

/**
 * Produce an identifier for a testsuite name.
 *
 * @param ID is the unique identifier part.
 */
#define TPP_INTERN_API_SUITE_NAME(ID) TPP_INTERN_CONCAT3(tpp_intern_suite_, ID, _)

/**
 * Generate a unique testsuite definition.
 *
 * @param DESCR is a cstring with the description of the testsuite.
 * @param BASE is the testsuite base type to use.
 */
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

/**
 * Generate a unique testcase definition as part of a testsuite.
 *
 * @param DESCR is a cstring with the description of the testcase.
 */
#define TPP_INTERN_API_TEST_WRAPPER(DESCR)                                                          \
    class TPP_INTERN_API_TEST_NAME(__LINE__)                                                        \
    {                                                                                               \
    public:                                                                                         \
        explicit TPP_INTERN_API_TEST_NAME(__LINE__)(tpp_intern_mod_type_ * mod_) {                  \
            mod_->tpp_intern_ts_()->test(DESCR, [=] { mod_->TPP_INTERN_API_TEST_FN(__LINE__)(); }); \
        }                                                                                           \
    } TPP_INTERN_API_TEST_INST(__LINE__){this};                                                     \
    void TPP_INTERN_API_TEST_FN(__LINE__)()

/**
 * Generate a non-unique function definition as part of a testsuite.
 *
 * @param FN is the identifier to use as function name part.
 */
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
 * Create a testsuite, where all testcases are run sequentially.
 *
 * EXAMPLE:
 * @code
 * SUITE("test some stuff") {
 *   // testcases
 * };
 * @endcode
 *
 * @param DESCR is a cstring with the description, or name of the testsuite.
 */
#define SUITE(DESCR) TPP_INTERN_API_SUITE_WRAPPER(DESCR, testsuite)

/**
 * Create a testsuite, where all testcases are run in parallel.
 *
 * EXAMPLE:
 * @code
 * SUITE_PAR("test some stuff in parallel") {
 *   // testcases
 * };
 * @endcode
 *
 * @param DESCR is a cstring with the description, or name of the testsuite.
 */
#define SUITE_PAR(DESCR) TPP_INTERN_API_SUITE_WRAPPER(DESCR, testsuite_parallel)

/// Synonym for SUITE.
#define DESCRIBE(DESCR) SUITE(DESCR)

/// Synonym for SUITE_PAR.
#define DESCRIBE_PAR(DESCR) SUITE_PAR(DESCR)

/**
 * Create a testcase.
 *
 * EXAMPLE:
 * @code
 * TEST("some test") {
 *   // assertions
 * }
 * @endcode
 *
 * @param DESCR is a cstring with the description, or name of the testcase.
 */
#define TEST(DESCR) TPP_INTERN_API_TEST_WRAPPER(DESCR)

/// Synonym for TEST, but DESCR is prefixed by 'It'.
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
