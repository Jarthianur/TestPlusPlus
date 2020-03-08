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

#ifndef SCTF_REGEX_HPP
#define SCTF_REGEX_HPP

#include <cstddef>
#include <regex>

namespace sctf
{
struct regex final
{
    regex(char const* p_, std::regex_constants::syntax_option_type flags_)
        : pattern(p_), re(p_, flags_) {}

    operator std::regex() const {
        return re;
    }

    char const* const pattern;
    std::regex const  re;
};

inline regex operator"" _re(char const* lit_, std::size_t) {
    return regex(lit_, std::regex::nosubs | std::regex::ECMAScript);
}

inline regex operator"" _re_i(char const* lit_, std::size_t) {
    return regex(lit_, std::regex::nosubs | std::regex::ECMAScript | std::regex::icase);
}

}  // namespace sctf

#endif  // SCTF_REGEX_HPP
