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

#ifndef TEST_TRAITS_HPP
#define TEST_TRAITS_HPP

#include <stdexcept>

#include "traits.hpp"

/*
 * Helpers for traits
 */

// void type

struct void_type
{};

// is_streamable

struct streamable
{
    template<typename S>
    friend auto
    operator<<(S& s_, streamable const&) -> S& {
        return s_;
    }
};

struct not_streamable
{};

template<typename S, typename T, SCTF_INTERN_ENABLE_IF(!SCTF_INTERN_HAS_STREAM_CAPABILITY(T, S))>
void
throw_if_not_streamable() {
    throw std::logic_error("Given type is not streamable");
}

template<typename S, typename T, SCTF_INTERN_ENABLE_IF(SCTF_INTERN_HAS_STREAM_CAPABILITY(T, S))>
void
throw_if_not_streamable() {}

// is_iterable

struct iterable
{
    struct iterator
    {
        auto
        operator!=(iterator const&) const noexcept -> bool {
            return false;
        }

        auto
        operator++() -> iterator& {
            return *this;
        }

        auto operator*() const noexcept -> bool {
            return true;
        }
    };

    static auto
    begin() noexcept -> iterator {
        return {};
    }

    static auto
    end() noexcept -> iterator {
        return {};
    }
};

struct not_iterable
{
    struct iterator
    {
        auto
        operator!=(iterator const&) const noexcept -> bool = delete;
        auto
             operator++() -> iterator&          = delete;
        auto operator*() const noexcept -> bool = delete;
    };

    static auto
    begin() noexcept -> iterator {
        return {};
    }

    static auto
    end() noexcept -> iterator {
        return {};
    }
};

template<typename T, SCTF_INTERN_ENABLE_IF(!SCTF_INTERN_HAS_ITERATOR_CAPABILITY(T))>
void
throw_if_not_iterable() {
    throw std::logic_error("Given type is not streamable");
}

template<typename T, SCTF_INTERN_ENABLE_IF(SCTF_INTERN_HAS_ITERATOR_CAPABILITY(T))>
void
throw_if_not_iterable() {}

#endif  // TEST_TRAITS_HPP
