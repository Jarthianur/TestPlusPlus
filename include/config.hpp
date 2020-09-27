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

#ifndef SCTF_CONFIG_HPP
#define SCTF_CONFIG_HPP

#include <regex>
#include <vector>

#include "reporter/reporter_factory.hpp"

namespace sctf
{
namespace intern
{
struct config
{
    enum class filter_mode
    {
        INCLUDE,
        EXCLUDE,
        NONE
    };

    enum class report_format
    {
        XML,
        MD,
        CNS
    };

    auto
    reporter() const -> reporter_ptr {
        switch (rep_fmt) {
            case report_format::XML: return reporter_factory::make<xml_reporter>(rep_cfg);
            case report_format::MD: return reporter_factory::make<markdown_reporter>(rep_cfg);
            default /*CNS*/: return reporter_factory::make<console_reporter>(rep_cfg);
        }
    }

    report_format           rep_fmt = report_format::CNS;
    reporter_config         rep_cfg;
    std::vector<std::regex> fpattern;
    filter_mode             fmode = filter_mode::NONE;
};
}  // namespace intern

using config = intern::config;
}  // namespace sctf

#endif  // SCTF_CONFIG_HPP
