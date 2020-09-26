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

#ifndef SCTF_RUNNER_HPP
#define SCTF_RUNNER_HPP

#include <algorithm>
#include <vector>

#include "reporter/reporter.hpp"
#include "testsuite/testsuite.hpp"
#include "testsuite/testsuite_parallel.hpp"

#include "cmdline_parser.hpp"

namespace sctf
{
/**
 * Used to manage and run testsuites.
 */
class runner
{
public:
    runner(int argc_, char** argv_) : m_cmdline(intern::tokenize_args(argc_, argv_)) {}

    /**
     * Add a testsuite to this runner.
     *
     * @param ts_ is the testsuite to add
     */
    void
    add_testsuite(intern::testsuite_ptr const& ts_) {
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
    run() noexcept -> std::size_t {
        auto rep = m_cmdline.reporter();
        rep->begin_report();
        std::for_each(m_testsuites.begin(), m_testsuites.end(), [&rep](intern::testsuite_ptr& ts_) {
            ts_->run();
            rep->report(ts_);
        });
        rep->end_report();
        return rep->faults();
    }

    /**
     * Get a runner instance, as singleton.
     */
    static auto
    instance(int argc_, char** argv_) -> runner& {
        static runner r(argc_, argv_);
        return r;
    }

private:
    std::vector<intern::testsuite_ptr> m_testsuites;  ///< Testsuites contained in this runner.
    intern::cmdline_parser             m_cmdline;
};
}  // namespace sctf

#endif  // SCTF_RUNNER_HPP
