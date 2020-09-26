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
#include <regex>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "reporter/reporter_factory.hpp"

namespace sctf
{
namespace intern
{
static auto
tokenize_args(int argc_, char** argv_) -> std::vector<std::string> {
    std::vector<std::string> a(argc_ - 1);
    for (std::size_t i = 1; i < argc_; ++i) {
        std::string arg(argv_[i]);
        if (arg.empty()) {
            continue;
        }
        a.push_back(std::move(arg));
    }
    return a;
}

class cmdline_parser
{
public:
    explicit cmdline_parser(std::vector<std::string> const& args_) {
        parse(args_);
    }

    void
    parse(std::vector<std::string> const& args_) {
        for (std::size_t i = 0; i < args_.size(); ++i) {
            auto const& arg     = args_[i];
            auto const  get_val = [&] {
                try {
                    return args_.at(++i);
                } catch (std::out_of_range const&) {
                    throw std::runtime_error(arg + " requires an argument!");
                }
            };

            if (arg == "--xml") {
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
        return std::regex(std::regex_replace(str_, std::regex("*"), ".*"),
                          std::regex_constants::nosubs | std::regex_constants::basic);
    }

    report_format           m_rep_fmt = CNS;
    reporter_config         m_rep_cfg;
    std::vector<std::regex> m_filters;
    filter_mode             m_filter_mode = filter_mode::NONE;
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_CMDLINE_PARSER_HPP
