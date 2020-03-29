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

/// @file

#ifndef SCTF_CPP_META_HPP
#define SCTF_CPP_META_HPP

#if __cplusplus >= 201703L
/// C++17 enabled
#    define SCTF_CPP_V17

#elif __cplusplus >= 201402L
/// C++14 enabled
#    define SCTF_CPP_V14

#elif __cplusplus >= 201103L
/// C++11 enabled
#    define SCTF_CPP_V11

#else

#    error SCTF requires at least full C++11 compliance

#endif

#if defined(__GNUG__) || defined(__clang__)
/// UNIX system (gcc/clang)
#    define SCTF_SYS_UNIX

#elif defined(_WIN32)
/// Windows system (msvc)
#    define SCTF_SYS_WIN

#else

#    error SCTF is only supported for Linux (gcc), OSX (clang), and Windows (msvc)

#endif

#endif  // SCTF_CPP_META_HPP
