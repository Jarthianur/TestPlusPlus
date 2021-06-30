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

#ifndef TPP_RUNNER_HPP
#define TPP_RUNNER_HPP

#include <algorithm>
#include <vector>

#include "report/reporter.hpp"
#include "test/testsuite.hpp"
#include "test/testsuite_parallel.hpp"

#include "cmdline_parser.hpp"
#include "omp.hpp"

namespace tpp
{
namespace intern
{
class runner
{
    enum class retval : int
    {
        HELP   = -1,
        EXCEPT = -2
    };

public:
    void
    add_testsuite(test::testsuite_ptr const& ts_) {
        m_testsuites.push_back(ts_);
    }

    auto
    run(int argc_, char const** argv_) noexcept -> int {
        cmdline_parser cmd;
        try {
            if (argc_ < 0) {
                throw std::runtime_error("argument count cannot be less than zero");
            }
            cmd.parse(static_cast<std::size_t>(argc_), argv_);
        } catch (cmdline_parser::help_called) {
            return to_int(retval::HELP);
        } catch (std::runtime_error const& e) {
            return err_exit(e.what());
        }
        return run(cmd.config());
    }

    auto
    run(config const& cfg_) noexcept -> int {
        omp_set_num_threads(cfg_.thd_count);
        bool const fm_inc{cfg_.f_mode != config::filter_mode::EXCLUDE};
        try {
            auto rep{cfg_.reporter()};
            rep->begin_report();
            std::for_each(m_testsuites.begin(), m_testsuites.end(), [&](test::testsuite_ptr& ts_) {
                bool const match{
                  cfg_.f_patterns.empty() ||
                  std::any_of(cfg_.f_patterns.cbegin(), cfg_.f_patterns.cend(),
                              [&](std::regex const& re_) -> bool { return std::regex_match(ts_->name(), re_); })};
                if (fm_inc == match) {
                    ts_->run();
                    rep->report(ts_);
                }
            });
            rep->end_report();
            return static_cast<int>(std::min(rep->faults(), static_cast<std::size_t>(std::numeric_limits<int>::max())));
        } catch (std::exception const& e) {
            return err_exit(e.what());
        } catch (...) {
            return err_exit("unknown error");
        }
    }

    static auto
    instance() -> runner& {
        static runner r;
        return r;
    }

private:
    static inline auto
    to_int(retval v_) -> int {
        return static_cast<int>(v_);
    }

    static inline auto
    err_exit(char const* msg_) -> int {
        std::cerr << "A fatal error occurred!\n  what(): " << msg_ << std::endl;
        return to_int(retval::EXCEPT);
    }

    std::vector<test::testsuite_ptr> m_testsuites;
};
}  // namespace intern

using runner = intern::runner;
}  // namespace tpp

#endif  // TPP_RUNNER_HPP
