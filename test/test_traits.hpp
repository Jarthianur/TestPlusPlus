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
    friend auto operator<<(S& s_, streamable const&) -> S& {
        return s_;
    }
};

struct not_streamable
{};

template<typename S, typename T, SCTF_INTERN_ENABLE_IF(!SCTF_INTERN_HAS_STREAM_CAPABILITY(T, S))>
void throw_if_not_streamable() {
    throw std::logic_error("Given type is not streamable");
}

template<typename S, typename T, SCTF_INTERN_ENABLE_IF(SCTF_INTERN_HAS_STREAM_CAPABILITY(T, S))>
void throw_if_not_streamable() {}

// is_iterable

struct iterable
{
    struct iterator
    {
        bool operator!=(iterator const&) const noexcept {
            return false;
        }

        iterator& operator++() {
            return *this;
        }

        bool operator*() const noexcept {
            return true;
        }
    };

    iterator begin() noexcept {
        return iterator();
    }

    iterator end() noexcept {
        return iterator();
    }
};

struct not_iterable
{
    struct iterator
    {
        bool      operator!=(iterator const&) const noexcept = delete;
        iterator& operator++()                               = delete;
        bool      operator*() const noexcept                 = delete;
    };

    iterator begin() noexcept {
        return iterator();
    }

    iterator end() noexcept {
        return iterator();
    }
};

template<typename T, SCTF_INTERN_ENABLE_IF(!SCTF_INTERN_HAS_ITERATOR_CAPABILITY(T))>
void throw_if_not_iterable() {
    throw std::logic_error("Given type is not streamable");
}

template<typename T, SCTF_INTERN_ENABLE_IF(SCTF_INTERN_HAS_ITERATOR_CAPABILITY(T))>
void throw_if_not_iterable() {}

#endif  // TEST_TRAITS_HPP
