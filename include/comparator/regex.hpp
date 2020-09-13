/*
    Copyright (C) 2017 Jarthianur

    This file is part of simple-cpp-test-framework.

    simple-cpp-test-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple-cpp-test-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple-cpp-test-framework.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SCTF_COMPARATOR_REGEX_HPP
#define SCTF_COMPARATOR_REGEX_HPP

#include <regex>

#include "comparator/comparator.hpp"

namespace sctf
{
namespace intern
{
/**
 * Comparator to check for regex match.
 */
template<typename T>
class match
{
    static constexpr char const* m_cmp_str     = "to be matching";
    static constexpr char const* m_neg_cmp_str = "to be not matching";
    bool                         m_neg         = false;
    T&                           m_res         = nullptr;

public:
    explicit match(T& res_) : m_res(res_) {}

    auto
    operator!() -> decltype(*this)& {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V, typename E = V>
    auto
    operator()(V const& actual_value, E const& expected_value) const -> comparison {
        return std::regex_match(actual_value, m_res, std::regex(expected_value)) != m_neg ?
                 comparison() :
                 comparison(m_neg ? m_neg_cmp_str : m_cmp_str,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }
};

template<>
class match<void>
{
    static constexpr char const* m_cmp_str     = "to be matching";
    static constexpr char const* m_neg_cmp_str = "to be not matching";
    bool                         m_neg         = false;

public:
    match() = default;

    auto
    operator!() -> decltype(*this)& {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V, typename E = V>
    auto
    operator()(V const& actual_value, E const& expected_value) const -> comparison {
        return std::regex_match(actual_value, std::regex(expected_value)) != m_neg ?
                 comparison() :
                 comparison(m_neg ? m_neg_cmp_str : m_cmp_str,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }
};

/**
 * Comparator to check for regex search.
 */
template<typename T>
class like
{
    static constexpr char const* m_cmp_str     = "to be like";
    static constexpr char const* m_neg_cmp_str = "to be not like";
    bool                         m_neg         = false;
    T&                           m_res         = nullptr;

public:
    explicit like(T& res_) : m_res(res_) {}

    auto
    operator!() -> decltype(*this)& {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V, typename E = V>
    auto
    operator()(V const& actual_value, E const& expected_value) const -> comparison {
        return std::regex_search(actual_value, m_res, std::regex(expected_value)) != m_neg ?
                 comparison() :
                 comparison(m_neg ? m_neg_cmp_str : m_cmp_str,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }
};

template<>
class like<void>
{
    static constexpr char const* m_cmp_str     = "to be like";
    static constexpr char const* m_neg_cmp_str = "to be not like";
    bool                         m_neg         = false;

public:
    like() = default;

    auto
    operator!() -> decltype(*this)& {
        m_neg = !m_neg;
        return *this;
    }

    template<typename V, typename E = V>
    auto
    operator()(V const& actual_value, E const& expected_value) const -> comparison {
        return std::regex_search(actual_value, std::regex(expected_value)) != m_neg ?
                 comparison() :
                 comparison(m_neg ? m_neg_cmp_str : m_cmp_str,
                            std::forward_as_tuple(to_string(actual_value), to_string(expected_value)));
    }
};
}  // namespace intern
}  // namespace sctf

SCTF_PROVIDE_COMPARATOR(match, MATCH)
SCTF_PROVIDE_COMPARATOR(like, LIKE)

#endif  // SCTF_COMPARATOR_REGEX_HPP
