/*
    Copyright (C) 2017 Jarthianur

    This file is part of TestPlusPlus (Test++).

    TestPlusPlus (Test++) is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TestPlusPlus (Test++) is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TestPlusPlus (Test++).  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TPP_RUNNER_HPP
#define TPP_RUNNER_HPP

#include <algorithm>
#include <vector>

#include "report/reporter.hpp"
#include "test/testsuite.hpp"
#include "test/testsuite_parallel.hpp"

#include "cmdline_parser.hpp"

namespace tpp
{
namespace intern
{
/**
 * Used to manage and run testsuites.
 */
class runner
{
public:
    /**
     * Add a testsuite to this runner.
     *
     * @param ts_ is the testsuite to add
     */
    void
    add_testsuite(test::testsuite_ptr const& ts_) {
        m_testsuites.push_back(ts_);
    }

    /**
     * Run all contained testsuites, what inherently performs all tests, and generates the report.
     * Tests are run only once, but reported every time this method is called.
     *
     * @param rep_ is the reporter to use for the report.
     * @return the sum of non successful tests.
     */
    auto
    run(int argc_, char** argv_) noexcept -> int {
        cmdline_parser cmd;
        try {
            cmd.parse(argc_, argv_);
        } catch (cmdline_parser::help_called) {
            return -1;
        }
        return run(cmd.config());
    }

    auto
    run(config const& cfg_) noexcept -> int {
        bool const finc = cfg_.fmode != config::filter_mode::EXCLUDE;
        try {
            auto rep = cfg_.reporter();
            rep->begin_report();
            std::for_each(m_testsuites.begin(), m_testsuites.end(), [&](test::testsuite_ptr& ts_) {
                bool const match = cfg_.fpattern.empty() || std::any_of(cfg_.fpattern.cbegin(), cfg_.fpattern.cend(),
                                                                        [&](std::regex const& re_) {
                                                                            return std::regex_match(ts_->name(), re_);
                                                                        });
                if (finc == match) {
                    ts_->run();
                    rep->report(ts_);
                }
            });
            rep->end_report();
            return static_cast<int>(rep->faults());  // always <= int::max
        } catch (std::runtime_error const& e) {
            std::cerr << "A fatal error occurred!\n  what(): " << e.what() << std::endl;
            return -2;
        }
    }

    /**
     * Get a runner instance, as singleton.
     */
    static auto
    instance() -> runner& {
        static runner r;
        return r;
    }

private:
    std::vector<test::testsuite_ptr> m_testsuites;  ///< Testsuites contained in this runner.
};
}  // namespace intern

using runner = intern::runner;
}  // namespace tpp

#endif  // TPP_RUNNER_HPP
