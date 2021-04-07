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

#ifndef TPP_CONFIG_HPP
#define TPP_CONFIG_HPP

#include <regex>
#include <vector>

#include "report/console_reporter.hpp"
#include "report/json_reporter.hpp"
#include "report/markdown_reporter.hpp"
#include "report/reporter_factory.hpp"
#include "report/xml_reporter.hpp"

#include "omp.hpp"

namespace tpp
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
        JSON,
        CNS
    };

    auto
    reporter() const -> reporter_ptr {
        switch (report_fmt) {
            case report_format::XML: return report::reporter_factory::make<xml_reporter>(report_cfg);
            case report_format::MD: return report::reporter_factory::make<markdown_reporter>(report_cfg);
            case report_format::JSON: return report::reporter_factory::make<json_reporter>(report_cfg);
            default /*CNS*/: return report::reporter_factory::make<console_reporter>(report_cfg);
        }
    }

    report_format           report_fmt{report_format::CNS};
    report::reporter_config report_cfg;
    std::vector<std::regex> f_patterns;
    filter_mode             f_mode{filter_mode::NONE};
    int                     thd_count = omp_get_max_threads();
};
}  // namespace intern

using config = intern::config;
}  // namespace tpp

#endif  // TPP_CONFIG_HPP
