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

#ifndef TPP_ASSERT_LOC_HPP
#define TPP_ASSERT_LOC_HPP

namespace tpp
{
namespace intern
{
namespace assert
{
struct loc final
{
    char const* file;  ///< The filename, where this location belongs to.
    int const   line;  ///< The line number in the file.
};
}  // namespace assert
}  // namespace intern
}  // namespace tpp

#endif  // TPP_ASSERT_LOC_HPP
