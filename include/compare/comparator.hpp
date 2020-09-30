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

#ifndef TPP_COMPARE_COMPARATOR_HPP
#define TPP_COMPARE_COMPARATOR_HPP

#include <memory>
#include <tuple>

#include "cpp_meta.hpp"
#include "stringify.hpp"

#ifdef TPP_INTERN_CPP_V17

#    include <optional>

#else

#    include <cstring>

#endif

namespace tpp
{
namespace intern
{
namespace compare
{
/**
 * Result of an actual comparison performed by any comparator.
 */
struct comparison final
{
#ifdef TPP_INTERN_CPP_V17

    comparison() : m_failure(std::nullopt) {}

    comparison(char const* comp_str_, std::tuple<std::string&&, std::string&&>&& val_)
        : m_failure("Expected " + std::get<0>(val_) + " " + comp_str_ + " " + std::get<1>(val_)) {}

    explicit operator bool() {
        return !m_failure;
    }

    auto
    operator*() const -> std::string const& {
        return *m_failure;
    }

private:
    std::optional<std::string> const m_failure;

#else

    comparison() = default;

    comparison(char const* comp_str_, std::tuple<std::string&&, std::string&&>&& val_) : m_success(false) {
        std::string msg;
        msg.reserve(15 + std::strlen(comp_str_) + std::get<0>(val_).length() + std::get<1>(val_).length());
        msg = "Expected ";
        msg.append(std::get<0>(val_)).append(" ").append(comp_str_).append(" ").append(std::get<1>(val_));
        error() = msg;
    }

    explicit operator bool() const {
        return m_success;
    }

    auto
    operator*() const -> std::string const& {
        return error();
    }

private:
    bool const m_success = true;

    static auto
    error() -> std::string& {
        static thread_local std::string err_msg;
        return err_msg;
    }

#endif
};
}  // namespace compare
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
#define TPP_COMPARATOR(NAME, CMPSTR, PRED)                                                                  \
    namespace tpp                                                                                           \
    {                                                                                                       \
    namespace intern                                                                                        \
    {                                                                                                       \
    namespace compare                                                                                       \
    {                                                                                                       \
    template<typename>                                                                                      \
    class NAME                                                                                              \
    {                                                                                                       \
        static constexpr char const* m_cmp_str     = "to be " CMPSTR;                                       \
        static constexpr char const* m_neg_cmp_str = "to be not " CMPSTR;                                   \
        bool                         m_neg         = false;                                                 \
                                                                                                            \
    public:                                                                                                 \
        auto                                                                                                \
        operator!() -> decltype(*this)& {                                                                   \
            m_neg = !m_neg;                                                                                 \
            return *this;                                                                                   \
        }                                                                                                   \
        template<typename V, typename E = V>                                                                \
        auto                                                                                                \
        operator()(V const& actual_value, E const& expected_value) const -> comparison {                    \
            return (PRED) != m_neg ?                                                                        \
                     comparison() :                                                                         \
                     comparison(m_neg ? m_neg_cmp_str : m_cmp_str,                                          \
                                std::forward_as_tuple(to_string(actual_value), to_string(expected_value))); \
        }                                                                                                   \
    };                                                                                                      \
    }                                                                                                       \
    }                                                                                                       \
    }

/**
 * Provide a shortwrite function which returns the respective
 * comparator. This shortwrite can then be used in assertions.
 *
 * @param COMP is the comparator to use.
 * @param NAME is the shortwrite function name.
 */
#define TPP_PROVIDE_COMPARATOR(COMP, NAME)                                  \
    namespace tpp                                                           \
    {                                                                       \
    template<typename... Args>                                              \
    static auto                                                             \
    NAME(Args&&... args) -> intern::compare::COMP<Args...> {                \
        return intern::compare::COMP<Args...>(std::forward<Args>(args)...); \
    }                                                                       \
    template<typename... Args>                                              \
    static auto                                                             \
    NAME() -> intern::compare::COMP<void> {                                 \
        return {};                                                          \
    }                                                                       \
    }

#endif  // TPP_COMPARE_COMPARATOR_HPP
