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
    /// The filename, where this location belongs to.
    char const* file;
    /// The line number in the file.
    int const line;
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_LOC_HPP
