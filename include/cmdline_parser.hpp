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
#include <stdexcept>

#include "config.hpp"
#include "version.hpp"

namespace sctf
{
namespace intern
{
class cmdline_parser
{
public:
    struct help_called
    {};

    void
    parse(int argc_, char** argv_) {
        auto const args = tokenize_args(argc_, argv_);
        m_progname      = argv_[0];
        for (std::size_t i = 1; i < args.size(); ++i) {
            eval_arg(args[i], [&] {
                try {
                    return args.at(++i);
                } catch (std::out_of_range const&) {
                    throw std::runtime_error(args[i - 1] + " requires an argument!");
                }
            });
        }
    }

    auto
    config() -> config const& {
        return m_cfg;
    }

private:
    template<typename Fn>
    void
    eval_arg(std::string const& arg_, Fn&& getval_fn_) {
        try {
            make_option(+"--help")(arg_, [&] { print_help(); });
            make_option(+"--xml")(arg_, [&] { m_cfg.rep_fmt = config::report_format::XML; });
            make_option(+"--md")(arg_, [&] { m_cfg.rep_fmt = config::report_format::MD; });
            make_option(+"--json")(arg_, [&] { m_cfg.rep_fmt = config::report_format::JSON; });
            combined_option{}(arg_, [&](char c_) {
                make_option('c')(c_, [&] { m_cfg.rep_cfg.color = true; });
                make_option('o')(c_, [&] { m_cfg.rep_cfg.capture_out = true; });
                make_option('s')(c_, [&] { m_cfg.rep_cfg.strip = true; });
                make_option('e')(c_, [&] {
                    set_filter_mode(config::filter_mode::EXCLUDE);
                    m_cfg.fpattern.push_back(to_regex(getval_fn_()));
                });
                make_option('i')(c_, [&] {
                    set_filter_mode(config::filter_mode::INCLUDE);
                    m_cfg.fpattern.push_back(to_regex(getval_fn_()));
                });
            });
        } catch (matched) {
            return;
        }
        m_cfg.rep_cfg.outfile = arg_;
    }

    struct matched
    {};

    template<typename T>
    struct option
    {
        T m_flag;

        template<typename Arg, typename Fn>
        auto
        operator()(Arg&& arg_, Fn&& fn_) const -> decltype(*this)& {
            if (arg_ == m_flag) {
                fn_();
                throw matched{};
            }
            return *this;
        }
    };

    struct combined_option
    {
        template<typename Fn>
        auto
        operator()(std::string const& arg_, Fn&& fn_) const -> decltype(*this)& {
            if (arg_[0] == '-') {
                std::for_each(arg_.cbegin() + 1, arg_.cend(), [&](char c_) {
                    try {
                        fn_(c_);
                    } catch (matched) {
                    }
                });
                throw matched{};
            }
            return *this;
        }
    };

    template<typename T>
    static inline auto
    make_option(T&& t_) -> option<T> {
        return option<T>{std::forward<T>(t_)};
    }

    void
    print_help() {
        std::cout << "Unit testing binary built with simple-cpp-test-framework (" SCTF_VERSION ").\n"
                  << "GitHub: https://github.com/Jarthianur/simple-cpp-test-framework\n\n"
                  << "Usage: " << m_progname << " [OPTIONS] [filename]\n"
                  << "Default is to report to standard-out in an informative text format (using console-reporter).\n\n"
                     "OPTIONS:\n"
                     "  --help: Print this message and exit.\n"
                     "  --xml : Report in JUnit-like XML format.\n"
                     "  --md  : Report in markdown format.\n"
                     "  --json: Report in json format.\n"
                     "  -c    : Use ANSI colors in report, if supported by reporter.\n"
                     "  -s    : Strip unnecessary whitespaces from report.\n"
                     "  -o    : Report captured output from tests, if supported by reporter.\n\n"
                     "  Multiple filters are possible, but includes and excludes are mutually exclusive.\n"
                     "  Patterns may contain * as wildcard.\n\n"
                     "  -e <pattern> : Exclude testsuites with names matching pattern.\n"
                     "  -i <pattern> : Include only testsuites with names matching pattern."
                  << std::endl;
        throw help_called{};
    }

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

    void
    set_filter_mode(config::filter_mode m_) {
        if (m_cfg.fmode == config::filter_mode::NONE) {
            m_cfg.fmode = m_;
        } else if (m_cfg.fmode != m_) {
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

    struct config m_cfg;
    char const*   m_progname;
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_CMDLINE_PARSER_HPP
