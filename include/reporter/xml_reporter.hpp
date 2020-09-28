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

#ifndef SCTF_REPORTER_XML_REPORTER_HPP
#define SCTF_REPORTER_XML_REPORTER_HPP

#include <array>
#include <chrono>
#include <ctime>
#include <memory>

#include "reporter/reporter.hpp"

namespace sctf
{
namespace intern
{
/**
 * Reporter implementation with JUnit like XML format.
 */
class xml_reporter : public reporter
{
public:
    xml_reporter(xml_reporter const&)     = delete;
    xml_reporter(xml_reporter&&) noexcept = delete;
    ~xml_reporter() noexcept override     = default;
    auto
    operator=(xml_reporter const&) -> xml_reporter& = delete;
    auto
    operator=(xml_reporter&&) noexcept -> xml_reporter& = delete;

    /**
     * Create a xml reporter.
     *
     * @param stream_ is the stream where to print the report. (default: stdout)
     */
    static auto
    create(std::ostream& stream_ = std::cout) -> reporter_ptr {
        return std::make_shared<xml_reporter>(enable{}, stream_);
    }

    /**
     * Create a xml reporter.
     *
     * @param fname_ is the filename where to print the report.
     */
    static auto
    create(std::string const& fname_) -> reporter_ptr {
        return std::make_shared<xml_reporter>(enable{}, fname_);
    }

    /// Constructor for std::make_shared.
    explicit xml_reporter(enable, std::ostream& stream_) : reporter(stream_) {}

    /// Constructor for std::make_shared.
    explicit xml_reporter(enable, std::string const& fname_) : reporter(fname_) {}

private:
    void
    report_testsuite(testsuite_ptr const& ts_) override {
        std::time_t           stamp = std::chrono::system_clock::to_time_t(ts_->timestamp());
        std::array<char, 128> buff{};
        std::strftime(buff.data(), 127, "%FT%T", std::localtime(&stamp));
        push_indent();
        *this << newline() << "<testsuite id=\"" << m_id++ << "\" name=\"" << ts_->name() << "\" errors=\""
              << ts_->statistics().errors() << "\" tests=\"" << ts_->statistics().tests() << "\" failures=\""
              << ts_->statistics().failures() << R"(" skipped="0" time=")" << ts_->execution_duration()
              << "\" timestamp=\"" << buff.data() << "\">";

        reporter::report_testsuite(ts_);

        *this << newline() << "</testsuite>";
        pop_indent();
    }

    void
    report_testcase(testcase const& tc_) override {
        push_indent();
        *this << newline() << "<testcase name=\"" << tc_.name() << "\" classname=\"" << tc_.suite_name() << "\" time=\""
              << tc_.duration() << "\"";
        switch (tc_.state()) {
            case testcase::result::ERROR:
                *this << ">";
                push_indent();
                *this << newline() << "<error message=\"" << tc_.reason() << "\"></error>";
                pop_indent();
                print_system_out(tc_);
                *this << newline() << "</testcase>";
                break;
            case testcase::result::FAILED:
                *this << ">";
                push_indent();
                *this << newline() << "<failure message=\"" << tc_.reason() << "\"></failure>";
                pop_indent();
                print_system_out(tc_);
                *this << newline() << "</testcase>";
                break;
            default:
                if (capture()) {
                    *this << ">";
                    print_system_out(tc_);
                    *this << newline() << "</testcase>";
                } else {
                    *this << "/>";
                }
                break;
        }
        pop_indent();
    }

    void
    begin_report() override {
        reporter::begin_report();

        *this << R"(<?xml version="1.0" encoding="UTF-8" ?>)" << newline() << "<testsuites>";
    }

    void
    end_report() override {
        *this << newline() << "</testsuites>" << newline();
    }

    void
    print_system_out(testcase const& tc_) {
        if (!capture()) {
            return;
        }
        push_indent();
        *this << newline() << "<system-out>" << tc_.cout() << "</system-out>" << newline() << "<system-err>"
              << tc_.cerr() << "</system-err>";
        pop_indent();
    }

    std::size_t mutable m_id = 0;  ///< Report wide incremental ID for testsuites.
};
}  // namespace intern

using xml_reporter = intern::xml_reporter;
}  // namespace sctf

#endif  // SCTF_REPORTER_XML_REPORTER_HPP
