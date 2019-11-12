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

#include "common/traits.hpp"

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
    friend auto operator<<(S& s_, streamable const&) -> S&
    {
        return s_;
    }
};

struct not_streamable
{};

template<typename S, typename T, ENABLE_IF(NOT IS_STREAMABLE(T, S))>
void throw_if_not_streamable()
{
    throw std::logic_error("Given type is not streamable");
}

template<typename S, typename T, ENABLE_IF(IS_STREAMABLE(T, S))>
void throw_if_not_streamable()
{}

// is_iterable

struct iterable
{
    struct iterator
    {
        bool operator!=(iterator const&) const noexcept
        {
            return false;
        }

        iterator& operator++()
        {
            return *this;
        }

        bool operator*() const noexcept
        {
            return true;
        }
    };

    iterator begin() noexcept
    {
        return iterator();
    }

    iterator end() noexcept
    {
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

    iterator begin() noexcept
    {
        return iterator();
    }

    iterator end() noexcept
    {
        return iterator();
    }
};

template<typename T, ENABLE_IF(NOT IS_ITERABLE(T))>
void throw_if_not_iterable()
{
    throw std::logic_error("Given type is not streamable");
}

template<typename T, ENABLE_IF(IS_ITERABLE(T))>
void throw_if_not_iterable()
{}

// is_ordinal

struct ordinal
{
    ordinal() = default;

    ordinal(bool v_) : m_v(v_) {}

    bool operator<(ordinal const&) const noexcept
    {
        return m_v;
    }

    bool operator>(ordinal const&) const noexcept
    {
        return m_v;
    }

private:
    bool m_v = true;
};

struct not_ordinal
{
    bool operator<(ordinal const&) const noexcept = delete;
    bool operator>(ordinal const&) const noexcept = delete;
};

template<typename T, ENABLE_IF(NOT IS_ORDINAL(T))>
void throw_if_not_ordinal()
{
    throw std::logic_error("Given type is not ordinal");
}

template<typename T, ENABLE_IF(IS_ORDINAL(T))>
void throw_if_not_ordinal()
{}

// is_equal_comparable

struct equal_comparable
{
    equal_comparable() = default;

    equal_comparable(bool v_) : m_v(v_) {}

    bool operator==(equal_comparable const&) const noexcept
    {
        return m_v;
    }

private:
    bool m_v = true;
};

struct equal_unequal_comparable
{
    equal_unequal_comparable() = default;

    equal_unequal_comparable(bool v_) : m_v(v_) {}

    bool operator==(equal_unequal_comparable const&) const noexcept
    {
        return m_v;
    }

    bool operator!=(equal_unequal_comparable const&) const noexcept
    {
        return m_v;
    }

private:
    bool m_v = true;
};

struct not_equal_comparable
{
    bool operator==(equal_comparable const&) const noexcept = delete;
};

template<typename T, ENABLE_IF(NOT IS_EQUAL_COMPARABLE(T, T))>
void throw_if_not_equal_comparable()
{
    throw std::logic_error("Given type is not equal-comparable");
}

template<typename T, ENABLE_IF(IS_EQUAL_COMPARABLE(T, T))>
void throw_if_not_equal_comparable()
{}

// is_unequal_comparable

struct unequal_comparable
{
    unequal_comparable() = default;

    unequal_comparable(bool v_) : m_v(v_) {}

    bool operator!=(unequal_comparable const&) const noexcept
    {
        return m_v;
    }

private:
    bool m_v = true;
};

struct not_unequal_comparable
{
    bool operator!=(unequal_comparable const&) const noexcept = delete;
};

template<typename T, ENABLE_IF(NOT IS_UNEQUAL_COMPARABLE(T, T))>
void throw_if_not_unequal_comparable()
{
    throw std::logic_error("Given type is not unequal-comparable");
}

template<typename T, ENABLE_IF(IS_UNEQUAL_COMPARABLE(T, T))>
void throw_if_not_unequal_comparable()
{}

#endif  // TEST_TRAITS_HPP
