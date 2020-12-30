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

#ifndef TPP_CPP_META_HPP
#define TPP_CPP_META_HPP

#if __cplusplus >= 201703L
/// C++17 enabled
#    define TPP_INTERN_CPP_V17

#elif __cplusplus >= 201402L
/// C++14 enabled
#    define TPP_INTERN_CPP_V14

#elif __cplusplus >= 201103L
/// C++11 enabled
#    define TPP_INTERN_CPP_V11

#else

#    error Test++ requires at least full C++11 compliance

#endif

#if defined(__GNUG__) || defined(__clang__)
/// UNIX system (gcc/clang)
#    define TPP_INTERN_SYS_UNIX

#elif defined(_WIN32)
/// Windows system (msvc)
#    define TPP_INTERN_SYS_WIN

#else

#    error Test++ is only supported for Linux (gcc), OSX (clang), and Windows (msvc)

#endif

// Experimental feature, that allows atomic blocks.
// Can be enabled by -fgnu-tm in gcc.
#if __cpp_transactional_memory >= 201505

#    define TPP_INTERN_SYNC synchronized

#else

#    define TPP_INTERN_SYNC

#endif

#endif  // TPP_CPP_META_HPP
