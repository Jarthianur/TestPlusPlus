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

/// @file

#ifndef SCTF_REPORTER_REPORTER_FACTORY_HPP
#define SCTF_REPORTER_REPORTER_FACTORY_HPP

#include "console_reporter.hpp"
#include "markdown_reporter.hpp"
#include "xml_reporter.hpp"

namespace sctf
{
namespace intern
{
struct reporter_config
{
    bool        color       = false;
    bool        capture_out = false;
    std::string outfile;
};

class reporter_factory
{
public:
    template<typename T>
    static auto
    make(reporter_config const&) -> std::shared_ptr<T>;
};

template<>
inline auto
reporter_factory::make<console_reporter>(reporter_config const& cfg_) -> std::shared_ptr<console_reporter> {
    auto rep = cfg_.outfile.empty() ? console_reporter::create() : console_reporter::create(cfg_.outfile);
    if (cfg_.capture_out) {
        rep->with_captured_output();
    }
    if (cfg_.color) {
        rep->with_color();
    }
    return rep;
}

template<>
inline auto
reporter_factory::make<xml_reporter>(reporter_config const& cfg_) -> std::shared_ptr<xml_reporter> {
    auto rep = cfg_.outfile.empty() ? xml_reporter::create() : xml_reporter::create(cfg_.outfile);
    if (cfg_.capture_out) {
        rep->with_captured_output();
    }
    return rep;
}

template<>
inline auto
reporter_factory::make<markdown_reporter>(reporter_config const& cfg_) -> std::shared_ptr<markdown_reporter> {
    auto rep = cfg_.outfile.empty() ? markdown_reporter::create() : markdown_reporter::create(cfg_.outfile);
    if (cfg_.capture_out) {
        rep->with_captured_output();
    }
    return rep;
}
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_REPORTER_REPORTER_FACTORY_HPP
