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

#ifndef SCTF_LOC_HPP
#define SCTF_LOC_HPP

namespace sctf
{
namespace intern
{
/**
 * A line of code, specified by its line number and filename.
 */
struct loc final
{
    char const* file;  ///< The filename, where this location belongs to.
    int const   line;  ///< The line number in the file.
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_LOC_HPP
