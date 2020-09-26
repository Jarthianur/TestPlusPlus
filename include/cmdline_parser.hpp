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

#ifndef SCTF_CMDLINE_PARSER_HPP
#define SCTF_CMDLINE_PARSER_HPP

#include "reporter/reporter_factory.hpp"

namespace sctf
{
namespace intern
{
struct args
{
    int    argc;
    char** argv;
};

/*
possible capabilities:
+ run only certain testsuites, based on name filter (csl)
+ run all but certain testsuites, based on name filter (csl)
+ specify report format, including all options
*/

class cmdline_parser
{
public:
    explicit cmdline_parser(args const& args_) {
        parse(args_);
    }

    void
    parse(args const& args_) {
        for (std::size_t i = 0; i < args_.argc; ++i) {
            std::string arg(args_.argv[i]);
            if (arg == "--xml") {
            }
        }
    }

    auto
    reporter() const -> reporter_ptr {
        switch (m_report_format) {
            case XML: return reporter_factory::make<xml_reporter>(m_rep_cfg);
            case MD: return reporter_factory::make<markdown_reporter>(m_rep_cfg);
            default /*CNS*/: return reporter_factory::make<console_reporter>(m_rep_cfg);
        }
    }

private:
    enum report_format
    {
        XML,
        MD,
        CNS
    };

    report_format   m_report_format = CNS;
    reporter_config m_rep_cfg;
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_CMDLINE_PARSER_HPP
