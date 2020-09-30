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

#ifndef TEST_TRAITS_HPP
#define TEST_TRAITS_HPP

#include <stdexcept>

#include "tpp.hpp"

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

template<typename S, typename T, TPP_INTERN_ENABLE_IF(!TPP_INTERN_HAS_STREAM_CAPABILITY(T, S))>
void
throw_if_not_streamable() {
    throw std::logic_error("Given type is not streamable");
}

template<typename S, typename T, TPP_INTERN_ENABLE_IF(TPP_INTERN_HAS_STREAM_CAPABILITY(T, S))>
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

        auto
        operator*() const noexcept -> bool {
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
        operator++() -> iterator& = delete;
        auto
        operator*() const noexcept -> bool = delete;
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

template<typename T, TPP_INTERN_ENABLE_IF(!TPP_INTERN_HAS_ITERATOR_CAPABILITY(T))>
void
throw_if_not_iterable() {
    throw std::logic_error("Given type is not streamable");
}

template<typename T, TPP_INTERN_ENABLE_IF(TPP_INTERN_HAS_ITERATOR_CAPABILITY(T))>
void
throw_if_not_iterable() {}

#endif  // TEST_TRAITS_HPP
