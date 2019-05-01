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

#ifndef SCTF_SRC_TYPES_H_
#define SCTF_SRC_TYPES_H_

#include <functional>
#include <memory>

namespace sctf
{
namespace _
{
class AbstractReporter;

/**
 * @typedef test_function
 * @brief Function schema passed to tests.
 */
using test_function = std::function<void()>;

}  // namespace _

/**
 * @typedef AbstractReporter_shared
 * @brief Shared ptr to AbstractReporter
 */
using AbstractReporter_shared = std::shared_ptr<_::AbstractReporter>;

/**
 * @typedef TestSuite_shared
 * @brief Shared ptr to TestSuite.
 */
class TestSuite;
using TestSuite_shared = std::shared_ptr<TestSuite>;

}  // namespace sctf

#endif  // SCTF_SRC_TYPES_H_
