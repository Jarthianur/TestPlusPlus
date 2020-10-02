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

#ifndef TPP_REPORT_REPORTER_FACTORY_HPP
#define TPP_REPORT_REPORTER_FACTORY_HPP

#include "report/reporter.hpp"

#include "traits.hpp"

namespace tpp
{
namespace intern
{
namespace report
{
struct reporter_config
{
    bool        color{false};
    bool        capture_out{false};
    bool        strip{false};
    std::string outfile;
};

class reporter_factory
{
public:
    template<typename T>
    static auto
    make(reporter_config const& cfg_) -> reporter_ptr {
        static_assert(TPP_INTERN_IS(std::is_base_of, reporter, T),
                      "Cannot make a concrete reporter that is not derived from abstract reporter!");
        auto rep = cfg_.outfile.empty() ? T::create() : T::create(cfg_.outfile);
        if (cfg_.capture_out) {
            rep->with_captured_output();
        }
        if (cfg_.color) {
            rep->with_color();
        }
        if (cfg_.strip) {
            rep->with_stripping();
        }
        return rep;
    }
};
}  // namespace report
}  // namespace intern
}  // namespace tpp

#endif  // TPP_REPORT_REPORTER_FACTORY_HPP
