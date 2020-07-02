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

#ifndef SCTF_TESTSUITE_STREAMBUF_PROXY_OMP_HPP
#define SCTF_TESTSUITE_STREAMBUF_PROXY_OMP_HPP

#include <ostream>
#include <sstream>
#include <streambuf>
#include <vector>

#ifdef _OPENMP
#    include <omp.h>
#else
#    define omp_get_max_threads() 1
#    define omp_get_thread_num() 0
#endif

namespace sctf
{
namespace intern
{
/**
 * Streambuffer proxy to capture everything sent to a stream in a multithreaded context with
 * OpenMP. Every thread has its own underlying buffer, thus output is distinct to each thread.
 */
class streambuf_proxy_omp : public std::streambuf
{
/// Get the buffer for the current thread.
#define SCTF_INTERN_CURRENT_THREAD_BUFFER() \
    (m_thd_buffers.at(static_cast<std::size_t>(omp_get_thread_num())))

public:
    streambuf_proxy_omp(streambuf_proxy_omp const&)     = delete;
    streambuf_proxy_omp(streambuf_proxy_omp&&) noexcept = delete;
    auto
    operator=(streambuf_proxy_omp const&) -> streambuf_proxy_omp& = delete;
    auto
    operator=(streambuf_proxy_omp&&) noexcept -> streambuf_proxy_omp& = delete;

    /**
     * Replace the underlying buffer of the stream.
     * As long as this object lives, everything sent to be stream is captured.
     *
     * @param stream_ is the stream to capture from.
     */
    explicit streambuf_proxy_omp(std::ostream& stream_)
        : m_orig_buf(stream_.rdbuf(this)),
          m_orig_stream(stream_),
          m_thd_buffers(static_cast<std::size_t>(omp_get_max_threads())) {}

    /**
     * Restore the original buffer of the stream.
     * After that, the stream is in its original state.
     */
    ~streambuf_proxy_omp() noexcept override {
        m_orig_stream.rdbuf(m_orig_buf);
    }

    /**
     * Get the current buffer content for the executing thread.
     */
    auto
    str() const -> std::string {
        return SCTF_INTERN_CURRENT_THREAD_BUFFER().str();
    }

    /**
     * Clear the buffer for the executing thread.
     */
    void
    clear() {
        SCTF_INTERN_CURRENT_THREAD_BUFFER().str("");
    }

private:
    auto
    overflow(int_type c_) -> int_type override {
        return SCTF_INTERN_CURRENT_THREAD_BUFFER().sputc(
        std::stringbuf::traits_type::to_char_type(c_));
    }

    auto
    xsputn(char const* s_, std::streamsize n_) -> std::streamsize override {
        return SCTF_INTERN_CURRENT_THREAD_BUFFER().sputn(s_, n_);
    }

    std::streambuf*             m_orig_buf;     ///< Target streams original buffer.
    std::ostream&               m_orig_stream;  ///< Target stream
    std::vector<std::stringbuf> m_thd_buffers;  ///< Buffer to store captured output.
};
}  // namespace intern
}  // namespace sctf

#ifndef _OPENMP
#    undef omp_get_max_threads
#    undef omp_get_thread_num
#endif
#undef SCTF_INTERN_CURRENT_THREAD_BUFFER

#endif  // SCTF_TESTSUITE_STREAMBUF_PROXY_OMP_HPP
