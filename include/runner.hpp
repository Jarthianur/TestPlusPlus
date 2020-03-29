/*
 Copyright_License {

 Copyright (C) 2017 Julian P. Becht
 Author: Julian P. Becht

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License version 3
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 }
 */

#ifndef SCTF_RUNNER_HPP
#define SCTF_RUNNER_HPP

#include <algorithm>
#include <vector>

#include "testsuite/testsuite.hpp"
#include "testsuite/testsuite_parallel.hpp"

namespace sctf
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
    void add_testsuite(testsuite_ptr ts_) {
        m_testsuites.push_back(ts_);
    }

    /**
     * Run all contained testsuites, what inherently performs all tests.
     */
    void run() noexcept {
        std::for_each(m_testsuites.begin(), m_testsuites.end(),
                      [](testsuite_ptr& ts_) { ts_->run(); });
    }

    /**
     * Get the contained testsuites.
     */
    std::vector<testsuite_ptr> const& testsuites() {
        return m_testsuites;
    }

    /**
     * Get a runner instance, as singleton.
     */
    static runner& instance() {
        static runner r;
        return r;
    }

private:
    std::vector<testsuite_ptr> m_testsuites;  ///< Testsuites contained in this runner.
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_RUNNER_HPP
