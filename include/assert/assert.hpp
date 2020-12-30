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
namespace intern
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

template<typename T, typename Fn>
static auto
assert_throws(Fn&& fn_, char const* tname_, loc&& loc_) -> throwable<T> {
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

template<typename Fn, TPP_INTERN_ENABLE_IF(!TPP_INTERN_IS_VOID(decltype(std::declval<Fn>()())))>
static auto
assert_nothrow(Fn&& fn_, loc&& loc_) -> decltype(fn_()) {
    try {
        return fn_();
    } catch (std::exception const& e) {
        throw assertion_failure(
          "Expected no exception, caught " + to_string(e) + "(\"" + escaped_string(e.what()) + "\")", loc_);
    } catch (...) {
        throw assertion_failure("Expected no exception", loc_);
    }
}

template<typename Fn, TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_VOID(decltype(std::declval<Fn>()())))>
static void
assert_nothrow(Fn&& fn_, loc&& loc_) {
    try {
        fn_();
    } catch (std::exception const& e) {
        throw assertion_failure(
          "Expected no exception, caught " + to_string(e) + "(\"" + escaped_string(e.what()) + "\")", loc_);
    } catch (...) {
        throw assertion_failure("Expected no exception", loc_);
    }
}

template<typename Fn, TPP_INTERN_ENABLE_IF(!TPP_INTERN_IS_VOID(decltype(std::declval<Fn>()())))>
static auto
assert_runtime(Fn&& fn_, double max_ms_, loc&& loc_) -> decltype(fn_()) {
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

template<typename Fn, TPP_INTERN_ENABLE_IF(TPP_INTERN_IS_VOID(decltype(std::declval<Fn>()())))>
static void
assert_runtime(Fn&& fn_, double max_ms_, loc&& loc_) {
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
 * Invoke this macro to define an assertion.
 *
 * @param NAME is the unique name of the assertion.
 * @param CMPSTR is a word describing the asserted constraint.
 * @param PRED is the predicate to assert.
 *
 * EXAMPLE:
 * @code
 * TPP_DEFINE_ASSERTION(assert_less, "less than", v_ < e_)
 * @endcode
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
        NAME(V&& v_, E&& e_, bool neg_, loc&& loc_) {                                                                 \
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
 * Invoke this macro to provide a name for an assertion, which can be used in ASSERT calls.
 *
 * @param A is the assertion to provide a name for.
 * @param S is the name.
 *
 * EXAMPLE:
 * @code
 * TPP_PROVIDE_ASSERTION(assert_less, LT)
 * @endcode
 */
#define TPP_PROVIDE_ASSERTION(A, S)   \
    namespace tpp                     \
    {                                 \
    namespace assert                  \
    {                                 \
    using S = tpp::intern::assert::A; \
    }                                 \
    }

/**
 * Generic macro to compare two values based on the given assertion.
 *
 * @param V is the value in question.
 * @param A is the assertion to use.
 * @param ... is the expected value and other optional args, like epsilon.
 *
 * EXAMPLE:
 * @code
 * ASSERT(1, EQ, 1);
 * ASSERT(1., EQ, 1.2, 0.01);
 * @endcode
 */
#define ASSERT(V, A, ...)                                                      \
    tpp::intern::assert::make_assertion<tpp::assert::A>(V, __VA_ARGS__, false, \
                                                        tpp::intern::assert::loc{__FILE__, __LINE__})

/**
 * Generic macro to compare two values based on the given assertion with negated result.
 *
 * @param V is the value in question.
 * @param A is the assertion to use.
 * @param ... is the expected value and other optional args, like epsilon.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NOT(1, EQ, 2);
 * ASSERT_NOT(1., EQ, 2.2, 0.01);
 * @endcode
 */
#define ASSERT_NOT(V, A, ...)                                                   \
    tpp::intern::assert::make_assertion<tpp::assert::A>((V), __VA_ARGS__, true, \
                                                        tpp::intern::assert::loc{__FILE__, __LINE__})

/**
 * Assert a value to be true.
 *
 * @param V is the value in question.
 *
 * EXAMPLE:
 * @code
 * ASSERT_TRUE(true);
 * @endcode
 */
#define ASSERT_TRUE(V) ASSERT_EQ((V), true)

/**
 * Assert a value to be false.
 *
 * @param V is the value in question.
 *
 * EXAMPLE:
 * @code
 * ASSERT_FALSE(false);
 * @endcode
 */
#define ASSERT_FALSE(V) ASSERT_EQ((V), false)

/**
 * Assert a pointer to be nullptr.
 *
 * @param P is the pointer in question.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NULL(&var);
 * @endcode
 */
#define ASSERT_NULL(P)                                                                      \
    static_assert(TPP_INTERN_IS(std::is_pointer, std::remove_reference<decltype(P)>::type), \
                  "ASSERT_NULL may only be used with pointer types!");                      \
    ASSERT_EQ(static_cast<void const*>(P), nullptr)

/**
 * Assert a pointer to be not nullptr.
 *
 * @param P is the pointer in question.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NOT_NULL(&var);
 * @endcode
 */
#define ASSERT_NOT_NULL(P)                                                                  \
    static_assert(TPP_INTERN_IS(std::is_pointer, std::remove_reference<decltype(P)>::type), \
                  "ASSERT_NOT_NULL may only be used with pointer types!");                  \
    ASSERT_NOT_EQ(static_cast<void const*>(P), nullptr)

/**
 * Assert an expression to throw a specific throwable type.
 *
 * @param F is the expression.
 * @param T is the expected throwable type.
 * @return the instance of T that was caught.
 *
 * EXAMPLE:
 * @code
 * ASSERT_THROWS(func(), std::exception);
 * @endcode
 */
#define ASSERT_THROWS(F, T) \
    tpp::intern::assert::assert_throws<T>([&] { F; }, #T, tpp::intern::assert::loc{__FILE__, __LINE__}).cause()

/**
 * Assert an expression to not throw anything.
 *
 * @param F is the expression.
 * @return the return value of F, if there is any.
 *
 * EXAMPLE:
 * @code
 * ASSERT_NOTHROW(func());
 * @endcode
 */
#define ASSERT_NOTHROW(F) tpp::intern::assert::assert_nothrow([&] { F; }, tpp::intern::assert::loc{__FILE__, __LINE__})

/**
 * Assert an expression to run in certain amount of time.
 *
 * @param F is the expression.
 * @param M is the maximum amount of time in milliseconds.
 * @return the return value of F, if there is any.
 *
 * EXAMPLE:
 * @code
 * ASSERT_RUNTIME(func());
 * @endcode
 */
#define ASSERT_RUNTIME(F, M) \
    tpp::intern::assert::assert_runtime([&] { F; }, M, tpp::intern::assert::loc{__FILE__, __LINE__})

#endif  // TPP_ASSERT_ASSERT_HPP
