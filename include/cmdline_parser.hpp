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

#include <algorithm>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "reporter/reporter_factory.hpp"

namespace sctf
{
namespace intern
{
struct help_called
{};

inline void
print_help(std::string const& name_) {
    //* last flag overrides preceeding ones.
    //  * e/i are appending, but mutually exclusive.
    //* [--xml|--md] [-{co}] [-{e|i} "pattern"] [report-filename]

    std::cout << "Usage: " << name_
              << " [OPTIONS] [FILTERS] [filename]\n"
                 "Default is to report to standard-out in an informative text format (using console-reporter).\n\n"
                 "OPTIONS:\n"
                 "  --help: Print this message and exit.\n"
                 "  --xml : Report in JUnit-like XML format.\n"
                 "  --md  : Report in markdown format.\n"
                 "  -c    : Use ANSI colors in report, if supported by reporter.\n"
                 "  -o    : Report captured output from tests, if supported by reporter.\n\n"
                 "FILTERS:\n"
                 "Multiple filters are possible, but includes and excludes are mutually exclusive.\n"
                 "Patterns may contain * as wildcard.\n"
                 "  -e <pattern> : Exclude testsuites with names matching pattern.\n"
                 "  -i <pattern> : Include only testsuites with names matching pattern."
              << std::endl;
    throw help_called{};
}

class cmdline_parser
{
public:
    void
    parse(int argc_, char** argv_) {
        auto const args = tokenize_args(argc_, argv_);
        for (std::size_t i = 1; i < args.size(); ++i) {
            auto const& arg     = args[i];
            auto const  get_val = [&] {
                try {
                    return args.at(++i);
                } catch (std::out_of_range const&) {
                    throw std::runtime_error(arg + " requires an argument!");
                }
            };

            if (arg == "--help") {
                print_help(args.at(0));
            } else if (arg == "--xml") {
                m_rep_fmt = XML;
            } else if (arg == "--md") {
                m_rep_fmt = MD;
            } else if (arg[0] == '-') {
                std::for_each(arg.cbegin() + 1, arg.cend(), [&](char c_) {
                    if (c_ == 'c') {
                        m_rep_cfg.color = true;
                    } else if (c_ == 'o') {
                        m_rep_cfg.capture_out = true;
                    } else if (c_ == 'e') {
                        set_filter_mode(filter_mode::EXCLUDE);
                        m_filters.push_back(to_regex(get_val()));
                    } else if (c_ == 'i') {
                        set_filter_mode(filter_mode::INCLUDE);
                        m_filters.push_back(to_regex(get_val()));
                    }
                });
            } else {
                m_rep_cfg.outfile = arg;
            }
        }
    }

    auto
    reporter() const -> reporter_ptr {
        switch (m_rep_fmt) {
            case XML: return reporter_factory::make<xml_reporter>(m_rep_cfg);
            case MD: return reporter_factory::make<markdown_reporter>(m_rep_cfg);
            default /*CNS*/: return reporter_factory::make<console_reporter>(m_rep_cfg);
        }
    }

    enum class filter_mode
    {
        INCLUDE,
        EXCLUDE,
        NONE
    };

    auto
    filters() const -> std::tuple<std::vector<std::regex> const&, filter_mode> {
        return {m_filters, m_filter_mode};
    }

private:
    static auto
    tokenize_args(int argc_, char** argv_) -> std::vector<std::string> {
        if (argc_ < 1) {
            throw std::underflow_error("Too few arguments!");
        }
        auto                     argc = static_cast<std::size_t>(argc_);
        std::vector<std::string> a;
        a.reserve(argc);
        for (std::size_t i = 0; i < argc; ++i) {
            std::string arg(argv_[i]);
            if (arg.empty()) {
                continue;
            }
            a.push_back(std::move(arg));
        }
        return a;
    }

    enum report_format
    {
        XML,
        MD,
        CNS
    };

    void
    set_filter_mode(filter_mode m_) {
        if (m_filter_mode == filter_mode::NONE) {
            m_filter_mode = m_;
        } else if (m_filter_mode != m_) {
            throw std::runtime_error("Inclusion and exclusion are mutually exclusive!");
        }
    }

    static auto
    to_regex(std::string const& str_) -> std::regex {
        try {
            return std::regex(std::regex_replace(str_, std::regex("\\*"), ".*"),
                              std::regex_constants::nosubs | std::regex_constants::basic);
        } catch (std::regex_error const&) {
            throw std::runtime_error(str_ + " is not a valid pattern!");
        }
    }

    report_format           m_rep_fmt = CNS;
    reporter_config         m_rep_cfg;
    std::vector<std::regex> m_filters;
    filter_mode             m_filter_mode = filter_mode::NONE;
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_CMDLINE_PARSER_HPP
