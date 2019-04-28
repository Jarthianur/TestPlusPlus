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

#ifndef TEST_TRAITS_HPP_
#define TEST_TRAITS_HPP_

#include <stdexcept>

#include "../src/util/traits.hpp"

/*
 * Helpers for traits
 */

struct void_type;
struct streamable;
struct not_streamable;
struct iterable;
struct not_iterable;
struct ordinal;
struct not_ordinal;
struct equal_comparable;
struct equal_unequal_comparable;
struct not_equal_comparable;
struct unequal_comparable;
struct not_unequal_comparable;

template<typename S, typename T,
         typename std::enable_if<!sctf::util::is_streamable<S, T>::value>::type* = nullptr>
void throw_if_not_streamable();

template<typename S, typename T,
         typename std::enable_if<sctf::util::is_streamable<S, T>::value>::type* = nullptr>
void throw_if_not_streamable();

template<typename T, typename std::enable_if<!sctf::util::is_iterable<T>::value>::type* = nullptr>
void throw_if_not_iterable();

template<typename T, typename std::enable_if<sctf::util::is_iterable<T>::value>::type* = nullptr>
void throw_if_not_iterable();

template<typename T, typename std::enable_if<!sctf::util::is_ordinal<T>::value>::type* = nullptr>
void throw_if_not_ordinal();

template<typename T, typename std::enable_if<sctf::util::is_ordinal<T>::value>::type* = nullptr>
void throw_if_not_ordinal();

template<typename T,
         typename std::enable_if<!sctf::util::is_equal_comparable<T, T>::value>::type* = nullptr>
void throw_if_not_equal_comparable();

template<typename T,
         typename std::enable_if<sctf::util::is_equal_comparable<T, T>::value>::type* = nullptr>
void throw_if_not_equal_comparable();

template<typename T,
         typename std::enable_if<!sctf::util::is_unequal_comparable<T, T>::value>::type* = nullptr>
void throw_if_not_unequal_comparable();

template<typename T,
         typename std::enable_if<sctf::util::is_unequal_comparable<T, T>::value>::type* = nullptr>
void throw_if_not_unequal_comparable();

// void type

struct void_type
{};

// is_streamable

struct streamable
{
    template<typename S>
    friend auto operator<<(S& s, const streamable&) -> S&
    {
        return s;
    }
};

struct not_streamable
{
    template<typename S>
    friend auto operator<<(S& s, const not_streamable&) -> S& = delete;
};

template<typename S, typename T,
         typename std::enable_if<!sctf::util::is_streamable<S, T>::value>::type*>
void throw_if_not_streamable()
{
    throw std::logic_error("Given type is not streamable");
}

template<typename S, typename T,
         typename std::enable_if<sctf::util::is_streamable<S, T>::value>::type*>
void throw_if_not_streamable()
{}

// is_iterable

struct iterable
{
    struct iterator
    {
        bool operator!=(const iterator&) const noexcept
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
        bool      operator!=(const iterator&) const noexcept = delete;
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

template<typename T, typename std::enable_if<!sctf::util::is_iterable<T>::value>::type*>
void throw_if_not_iterable()
{
    throw std::logic_error("Given type is not streamable");
}

template<typename T, typename std::enable_if<sctf::util::is_iterable<T>::value>::type*>
void throw_if_not_iterable()
{}

// is_ordinal

struct ordinal
{
    ordinal() = default;

    ordinal(bool v) : _v(v) {}

    bool operator<(const ordinal&) const noexcept
    {
        return _v;
    }

    bool operator>(const ordinal&) const noexcept
    {
        return _v;
    }

private:
    bool _v = true;
};

struct not_ordinal
{
    bool operator<(const ordinal&) const noexcept = delete;
    bool operator>(const ordinal&) const noexcept = delete;
};

template<typename T, typename std::enable_if<!sctf::util::is_ordinal<T>::value>::type*>
void throw_if_not_ordinal()
{
    throw std::logic_error("Given type is not ordinal");
}

template<typename T, typename std::enable_if<sctf::util::is_ordinal<T>::value>::type*>
void throw_if_not_ordinal()
{}

// is_equal_comparable

struct equal_comparable
{
    equal_comparable() = default;

    equal_comparable(bool v) : _v(v) {}

    bool operator==(const equal_comparable&) const noexcept
    {
        return _v;
    }

private:
    bool _v = true;
};

struct equal_unequal_comparable
{
    equal_unequal_comparable() = default;

    equal_unequal_comparable(bool v) : _v(v) {}

    bool operator==(const equal_unequal_comparable&) const noexcept
    {
        return _v;
    }

    bool operator!=(const equal_unequal_comparable&) const noexcept
    {
        return _v;
    }

private:
    bool _v = true;
};

struct not_equal_comparable
{
    bool operator==(const equal_comparable&) const noexcept = delete;
};

template<typename T, typename std::enable_if<!sctf::util::is_equal_comparable<T, T>::value>::type*>
void throw_if_not_equal_comparable()
{
    throw std::logic_error("Given type is not equal-comparable");
}

template<typename T, typename std::enable_if<sctf::util::is_equal_comparable<T, T>::value>::type*>
void throw_if_not_equal_comparable()
{}

// is_unequal_comparable

struct unequal_comparable
{
    unequal_comparable() = default;

    unequal_comparable(bool v) : _v(v) {}

    bool operator!=(const unequal_comparable&) const noexcept
    {
        return _v;
    }

private:
    bool _v = true;
};

struct not_unequal_comparable
{
    bool operator!=(const unequal_comparable&) const noexcept = delete;
};

template<typename T,
         typename std::enable_if<!sctf::util::is_unequal_comparable<T, T>::value>::type*>
void throw_if_not_unequal_comparable()
{
    throw std::logic_error("Given type is not unequal-comparable");
}

template<typename T, typename std::enable_if<sctf::util::is_unequal_comparable<T, T>::value>::type*>
void throw_if_not_unequal_comparable()
{}

#endif  // TEST_TRAITS_HPP_
