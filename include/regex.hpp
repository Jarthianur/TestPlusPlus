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

#ifndef TPP_REGEX_HPP
#define TPP_REGEX_HPP

#include <cstddef>
#include <regex>

namespace tpp
{
namespace intern
{
struct regex final
{
    regex(char const* p_, std::regex_constants::syntax_option_type flags_) : pattern(p_), re(p_, flags_) {}

    operator std::regex() const {
        return re;
    }

    char const* const pattern;
    std::regex const  re;
};
}  // namespace intern

using regex = intern::regex;

/**
 * Literal operator to create a regex from cstring.
 * The produced regex uses ECMAScript syntax, and does not allow capturing.
 *
 * @param lit_ is the regular expression.
 * @return a regex built from the given pattern.
 *
 * EXAMPLE:
 * @code
 * ".*"_re
 * @endcode
 */
inline auto operator"" _re(char const* lit_, std::size_t) -> regex {
    return regex(lit_, std::regex::ECMAScript);
}

/**
 * Literal operator to create a regex from cstring.
 * The produced regex uses ECMAScript syntax, does not allow capturing, and is case insensitive.
 *
 * @param lit_ is the regular expression.
 * @return a regex built from the given pattern.
 *
 * EXAMPLE:
 * @code
 * ".*"_re_i
 * @endcode
 */
inline auto operator"" _re_i(char const* lit_, std::size_t) -> regex {
    return regex(lit_, std::regex::ECMAScript | std::regex::icase);
}
}  // namespace tpp

#endif  // TPP_REGEX_HPP
