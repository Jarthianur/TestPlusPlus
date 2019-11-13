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

#ifndef SCTF_TESTSUITE_RUNNER_HPP
#define SCTF_TESTSUITE_RUNNER_HPP

#include <algorithm>
#include <vector>

#include "common/stringify.hpp"
#include "common/types.hpp"
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
    testsuite_ptr register_ts(testsuite_ptr ts_)
    {
        m_testsuites.push_back(ts_);
        return ts_;
    }

    /**
     * @brief Run TestSuites.
     */
    void run() noexcept
    {
        std::for_each(m_testsuites.begin(), m_testsuites.end(),
                      [](testsuite_ptr& ts_) { ts_->run(); });
    }

    /**
     * @brief Get the TestSuites.
     * @return the testsuites
     */
    const std::vector<testsuite_ptr>& testsuites()
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
    std::vector<testsuite_ptr> m_testsuites;
};

/**
 * @brief Describe and register a TestSuite to the given runner.
 * @note All test cases in this suite will be executed in parallel.
 * @param name The name/description
 * @param context The component/context to test as string (default="")
 * @param runner The TestSuitesRunner to register
 * @return a shared pointer to the created TestSuite
 */
inline static testsuite_ptr describe_parallel(char const* name_, char const* ctx_ = "main",
                                              runner& runner_ = runner::default_instance())
{
    return runner_.register_ts(testsuite_parallel::create(name_, ctx_));
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
static testsuite_ptr describe_parallel(char const* name_,
                                       runner&     runner_ = runner::default_instance())
{
    return runner_.register_ts(testsuite_parallel::create(name_, _::name_for_type<T>()));
}

/**
 * @brief Describe and register a TestSuite to the given runner.
 * @note All test cases in this suite will be executed sequentially.
 * @param name The name/description
 * @param context The component/context to test as string (default="")
 * @param runner The TestSuitesRunner to register
 * @return a shared pointer to the created TestSuite
 */
inline static testsuite_ptr describe(char const* name_, char const* ctx_ = "main",
                                     runner& runner_ = runner::default_instance())
{
    return runner_.register_ts(testsuite::create(name_, ctx_));
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
static testsuite_ptr describe(char const* name_, runner& runner_ = runner::default_instance())
{
    return runner_.register_ts(testsuite::create(name_, _::name_for_type<T>()));
}
}  // namespace sctf

#endif  // SCTF_TESTSUITE_RUNNER_HPP
