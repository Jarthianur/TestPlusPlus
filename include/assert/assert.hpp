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

#ifndef TPP_ASSERT_ASSERT_HPP
#define TPP_ASSERT_ASSERT_HPP

#include <tuple>

#include "assert/assertion_failure.hpp"
#include "assert/loc.hpp"

#include "cpp_meta.hpp"
#include "duration.hpp"
#include "stringify.hpp"
#include "traits.hpp"

namespace tpp
{
template<typename T>
class throwable
{
public:
    explicit throwable(T const& t_) : m_t(t_) {}

    auto
    cause() const -> T const& {
        return m_t;
    }

private:
    T m_t;
};

namespace intern
{
namespace assert
{
template<typename A, typename... Args>
static inline auto
make_assertion(Args&&... args) -> A {
    return A{std::forward<Args>(args)...};
}

static inline void
fail_assertion(std::tuple<std::string&&, char const*, std::string&&>&& asrt_, loc const& loc_) {
    throw assertion_failure{std::string("Expected ")
                              .append(std::get<0>(asrt_))  // value
                              .append(" ")
                              .append(std::get<1>(asrt_))  // constraint
                              .append(" ")
                              .append(std::get<2>(asrt_)),  // expected
                            loc_};
}

/**
 * Apply a throw-assertion on a function invokation.
 *
 * @tparam T is the throwable type to expect.
 * @param fn_ is the wrapper function to assert for.
 * @param tname_ is the throwable types name.
 * @param loc_ is the line of code where the assertion took place.
 * @return the instance of T that was caught.
 * @throw tpp::intern::assertion_failure if a different throwable type, or nothing is thrown by
 * fn_.
 */
template<typename T, typename Fn>
auto
assert_throws(Fn&& fn_, char const* tname_, loc const& loc_) -> throwable<T> {
    try {
        fn_();
    } catch (T const& e) {
        return throwable<T>{e};
    } catch (std::exception const& e) {
        throw assertion_failure(
          "Wrong exception thrown, caught " + to_string(e) + "(\"" + escaped_string(e.what()) + "\")", loc_);
    } catch (...) {
        throw assertion_failure("Wrong exception thrown", loc_);
    }
    throw assertion_failure(std::string("No exception thrown, expected ") + tname_, loc_);
}

/**
 * Apply a nothrow-assertion on a function invokation.
 *
 * @param fn_ is the wrapper function to assert for.
 * @param loc_ is the line of code where the assertion took place.
 * @return the return value of fn_.
 * @throw tpp::intern::assertion_failure if a any throwable type is thrown by fn_.
 */
template<typename Fn, TPP_INTERN_ENABLE_IF(!TPP_INTERN_IS_VOID(decltype(std::declval<Fn>()())))>
auto
assert_nothrow(Fn&& fn_, loc const& loc_) -> decltype(fn_()) {
    try {
        return fn_();
    } catch (std::exception const& e) {
        throw assertion_failure(
          "Expected no exception, caught " + to_string(e) + "(\"" + escaped_string(e.what()) + "\")", loc_);
    } catch (...) {
        throw assertion_failure("Expected no exception", loc_);
    }
}

/**
 * Apply a nothrow-assertion on a function invokation.
 *
 * @param fn_ is the wrapper function to assert for.
 * @param loc_ is the line of code where the assertion took place.
 * @throw tpp::intern::assertion_failure if a any throwable type is thrown by fn_.
 */
template<typename Fn, TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_VOID(decltype(std::declval<Fn>()())))>
void
assert_nothrow(Fn&& fn_, loc const& loc_) {
    try {
        fn_();
    } catch (std::exception const& e) {
        throw assertion_failure(
          "Expected no exception, caught " + to_string(e) + "(\"" + escaped_string(e.what()) + "\")", loc_);
    } catch (...) {
        throw assertion_failure("Expected no exception", loc_);
    }
}

/**
 * Apply a runtime-assertion on a function invokation.
 *
 * @param fn_ is the wrapper function to assert for.
 * @param max_ms_ is the maximum amount of time in milliseconds.
 * @param loc_ is the line of code where the assertion took place.
 * @return the return value of fn_.
 * @throw tpp::intern::assertion_failure if fn_ does not complete within max_ms_.
 */
template<typename Fn, TPP_INTERN_ENABLE_IF(!TPP_INTERN_IS_VOID(decltype(std::declval<Fn>()())))>
auto
assert_runtime(Fn&& fn_, double max_ms_, loc const& loc_) -> decltype(fn_()) {
    TPP_INTERN_SYNC {
        duration        dur;
        decltype(fn_()) res{fn_()};
        double          dur_ms{dur.get()};
        if (dur_ms > max_ms_) {
            throw assertion_failure("Expected the runtime to be less " + to_string(max_ms_) + "ms, but was " +
                                      to_string(dur_ms) + "ms",
                                    loc_);
        }
        return res;
    }
}

/**
 * Apply a runtime-assertion on a function invokation.
 *
 * @param fn_ is the wrapper function to assert for.
 * @param max_ms_ is the maximum amount of time in milliseconds.
 * @param loc_ is the line of code where the assertion took place.
 * @throw tpp::intern::assertion_failure if fn_ does not complete within max_ms_.
 */
template<typename Fn, TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_VOID(decltype(std::declval<Fn>()())))>
void
assert_runtime(Fn&& fn_, double max_ms_, loc const& loc_) {
    double dur_ms{.0};
    TPP_INTERN_SYNC {
        duration dur;
        fn_();
        dur_ms = dur.get();
    }
    if (dur_ms > max_ms_) {
        throw assertion_failure(
          "Expected the runtime to be less " + to_string(max_ms_) + "ms, but was " + to_string(dur_ms) + "ms", loc_);
    }
}
}  // namespace assert
}  // namespace intern
}  // namespace tpp

/**
 * Define a comparator.
 * In PRED the two elements are named 'actual_value' and 'expected_value'.
 * The comparison is considered successful if PRED returns true, while false results in failure.
 *
 * @param NAME is the name of the comparator.
 * @param CMPSTR is a cstring representing the comparison constraint, like "equals".
 * @param PRED is the comparison predicate / condition.
 */
#define TPP_DEFINE_ASSERTION(NAME, CMPSTR, PRED)                                                                      \
    namespace tpp                                                                                                     \
    {                                                                                                                 \
    namespace intern                                                                                                  \
    {                                                                                                                 \
    namespace assert                                                                                                  \
    {                                                                                                                 \
    namespace ns_##NAME {                                                                                             \
        static constexpr auto CMP_STR     = "to be " CMPSTR;                                                          \
        static constexpr auto NEG_CMP_STR = "to be not " CMPSTR;                                                      \
    }                                                                                                                 \
    struct NAME                                                                                                       \
    {                                                                                                                 \
        template<typename V, typename E = V>                                                                          \
        NAME(V&& v_, E&& e_, bool neg_, loc const& loc_) {                                                            \
            if ((PRED) == neg_) {                                                                                     \
                fail_assertion(std::forward_as_tuple(                                                                 \
                                 to_string(v_), (neg_ ? ns_##NAME::NEG_CMP_STR : ns_##NAME::CMP_STR), to_string(e_)), \
                               loc_);                                                                                 \
            }                                                                                                         \
        }                                                                                                             \
    };                                                                                                                \
    }                                                                                                                 \
    }                                                                                                                 \
    }

/**
 * Generic assertion to compare two values.
 *
 * EXAMPLE:
 * @code
 * ASSERT(1, EQ(), 1);
 * @endcode
 *
 * @param VAL is the actual value.
 * @param CMP is the comparator to use.
 * @param EXP is the expected value.
 */
#define ASSERT(V, A, ...)                                                              \
    tpp::intern::assert::make_assertion<tpp::intern::assert::A>(V, __VA_ARGS__, false, \
                                                                tpp::intern::assert::loc{__FILE__, __LINE__})

#define ASSERT_NOT(V, A, ...)                                                         \
    tpp::intern::assert::make_assertion<tpp::intern::assert::A>(V, __VA_ARGS__, true, \
                                                                tpp::intern::assert::loc{__FILE__, __LINE__})

/**
 * Assert a value to be true.
 *
 * EXAMPLE:
 * @code
 * ASSERT_TRUE(true);
 * @endcode
 *
 * @param VAL is the actual value.
 */
#define ASSERT_TRUE(VAL) ASSERT_EQ(VAL, true)

/**
 * Assert a value to be false.
 *
 * EXAMPLE:
 * @code
 * ASSERT_FALSE(false);
 * @endcode
 *
 * @param VAL is the actual value.
 */
#define ASSERT_FALSE(VAL) ASSERT_EQ(VAL, false)

/**
 * Assert a pointer to be nullptr.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NULL(&var);
 * @endcode
 *
 * @param PTR is the actual pointer.
 */
#define ASSERT_NULL(PTR)                                                                      \
    static_assert(TPP_INTERN_IS(std::is_pointer, std::remove_reference<decltype(PTR)>::type), \
                  "ASSERT_NULL may only be used with pointer types!");                        \
    ASSERT_EQ(static_cast<void const*>(PTR), nullptr)

/**
 * Assert a pointer to be not nullptr.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NOT_NULL(&var);
 * @endcode
 *
 * @param PTR is the actual pointer.
 */
#define ASSERT_NOT_NULL(PTR)                                                                  \
    static_assert(TPP_INTERN_IS(std::is_pointer, std::remove_reference<decltype(PTR)>::type), \
                  "ASSERT_NOT_NULL may only be used with pointer types!");                    \
    ASSERT_NOT_EQ(static_cast<void const*>(PTR), nullptr)

/**
 * Assert an expression to throw a specific throwable type.
 *
 * EXAMPLE:
 * @code
 * ASSERT_THROWS(func(), std::exception);
 * @endcode
 *
 * @param FN is the expression / invokation.
 * @param TRW is the expected throwable type.
 * @return the instance of TRW that was caught.
 */
#define ASSERT_THROWS(FN, TRW) \
    tpp::intern::assert::assert_throws<TRW>([&] { FN; }, #TRW, tpp::intern::assert::loc{__FILE__, __LINE__}).cause()

/**
 * Assert an expression to not throw anything.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NOTHROW(func());
 * @endcode
 *
 * @param FN is the expression / invokation.
 * @return the return value of FN, if there is any.
 */
#define ASSERT_NOTHROW(FN) \
    tpp::intern::assert::assert_nothrow([&] { FN; }, tpp::intern::assert::loc{__FILE__, __LINE__})

/**
 * Assert an expression to run in certain amount of time.
 * This may be usefull, when testing fixed timing constraints.
 *
 * EXAMPLE:
 * @code
 * ASSERT_RUNTIME(func());
 * @endcode
 *
 * @param FN is the expression / invokation.
 * @param MAX is the maximum amount of time in milliseconds.
 * @return the return value of FN, if there is any.
 */
#define ASSERT_RUNTIME(FN, MAX) \
    tpp::intern::assert::assert_runtime([&] { FN; }, MAX, tpp::intern::assert::loc{__FILE__, __LINE__})

#endif  // TPP_ASSERT_ASSERT_HPP
