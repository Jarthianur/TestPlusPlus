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

#ifndef SCTF_TESTSUITE_RUNNER_HPP_
#define SCTF_TESTSUITE_RUNNER_HPP_

#include <algorithm>
#include <string>
#include <vector>

#include "common/stringify.hpp"
#include "common/types.h"
#include "testsuite/testsuite.hpp"
#include "testsuite/testsuite_parallel.hpp"

namespace sctf
{
/**
 * @brief A runner to manage and run TestSuites.
 */
class runner
{
public:
    runner()           = default;
    ~runner() noexcept = default;

    /**
     * @brief Register a TestSuite.
     * @param ts A shared ptr to the TestSuite
     * @return a shared pointer to the TestSuite
     */
    testsuite_shared register_ts(testsuite_shared ts)
    {
        m_testsuites.push_back(ts);
        return ts;
    }

    /**
     * @brief Run TestSuites.
     */
    void run() noexcept
    {
        std::for_each(m_testsuites.begin(), m_testsuites.end(),
                      [](testsuite_shared& ts) { ts->run(); });
    }

    /**
     * @brief Get the TestSuites.
     * @return the testsuites
     */
    const std::vector<testsuite_shared>& testsuites()
    {
        return m_testsuites;
    }

    /**
     * @brief Get an instance of a runner, which is used as default.
     * @return the default runner instance
     */
    static runner& default_instance()
    {
        static runner r;
        return r;
    }

private:
    /// @brief The registered TestSuites.
    std::vector<testsuite_shared> m_testsuites;
};

/**
 * @brief Describe and register a TestSuite to the given runner.
 * @note All test cases in this suite will be executed in parallel.
 * @param name The name/description
 * @param context The component/context to test as string (default="")
 * @param runner The TestSuitesRunner to register
 * @return a shared pointer to the created TestSuite
 */
inline static testsuite_shared describeParallel(const std::string& name,
                                                runner& runner = runner::default_instance(),
                                                const std::string& context = "")
{
    return runner.register_ts(testsuite_parallel::create(name, context));
}

/**
 * @brief Describe and register a TestSuite to the given runner.
 * @note All test cases in this suite will be executed in parallel.
 * @tparam T The component/context classname where this suite belongs to
 * @param  name The name/description
 * @param runner  The TestSuitesRunner to register
 * @return a shared pointer to the created TestSuite
 */
template<typename T>
static testsuite_shared describeParallel(const std::string& name,
                                         runner&            runner = runner::default_instance())
{
    return runner.register_ts(testsuite_parallel::create(name, _::name_for_type<T>()));
}

/**
 * @brief Describe and register a TestSuite to the given runner.
 * @note All test cases in this suite will be executed sequentially.
 * @param name The name/description
 * @param context The component/context to test as string (default="")
 * @param runner The TestSuitesRunner to register
 * @return a shared pointer to the created TestSuite
 */
inline static testsuite_shared describe(const std::string& name,
                                        runner&            runner  = runner::default_instance(),
                                        const std::string& context = "")
{
    return runner.register_ts(testsuite::create(name, context));
}

/**
 * @brief Describe and register a TestSuite to the given runner.
 * @note All test cases in this suite will be executed sequentially.
 * @tparam T The component/context classname where this suite belongs to
 * @param  name The name/description
 * @param runner  The TestSuitesRunner to register
 * @return a shared pointer to the created TestSuite
 */
template<typename T>
static testsuite_shared describe(const std::string& name,
                                 runner&            runner = runner::default_instance())
{
    return runner.register_ts(testsuite::create(name, _::name_for_type<T>()));
}

}  // namespace sctf

#endif  // SCTF_TESTSUITE_RUNNER_HPP_
