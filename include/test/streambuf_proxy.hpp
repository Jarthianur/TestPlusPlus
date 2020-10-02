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

/// @file

#ifndef TPP_TEST_STREAMBUF_PROXY_HPP
#define TPP_TEST_STREAMBUF_PROXY_HPP

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

namespace tpp
{
namespace intern
{
namespace test
{
class streambuf_proxy : public std::streambuf
{
public:
    streambuf_proxy(streambuf_proxy const&)     = delete;
    streambuf_proxy(streambuf_proxy&&) noexcept = delete;
    auto
    operator=(streambuf_proxy const&) -> streambuf_proxy& = delete;
    auto
    operator=(streambuf_proxy&&) noexcept -> streambuf_proxy& = delete;

    /**
     * Replace the underlying buffer of the stream.
     * As long as this object lives, everything sent to be stream is captured.
     *
     * @param stream_ is the stream to capture from.
     */
    explicit streambuf_proxy(std::ostream& stream_) : m_orig_buf(stream_.rdbuf(this)), m_orig_stream(stream_) {}

    /**
     * Restore the original buffer of the stream.
     * After that, the stream is in its original state.
     */
    ~streambuf_proxy() noexcept override {
        m_orig_stream.rdbuf(m_orig_buf);
    }

    /**
     * Get the current buffer content and clear it.
     */
    virtual auto
    str() -> std::string = 0;

protected:
    template<typename T>
    class clearer
    {
        T* m_buf;

    public:
        clearer(clearer const&)     = delete;
        clearer(clearer&&) noexcept = delete;
        auto
        operator=(clearer const&) -> clearer& = delete;
        auto
        operator=(clearer&&) noexcept -> clearer& = delete;

        explicit clearer(T* b_) : m_buf(b_) {}

        ~clearer() noexcept {
            m_buf->str("");
        }
    };

    std::streambuf* m_orig_buf;     ///< Target streams original buffer.
    std::ostream&   m_orig_stream;  ///< Target stream
};

/**
 * Streambuffer proxy to capture everything sent to a stream.
 */
class streambuf_proxy_single : public streambuf_proxy
{
public:
    explicit streambuf_proxy_single(std::ostream& stream_) : streambuf_proxy(stream_) {}

    auto
    str() -> std::string override {
        clearer<std::stringbuf> _(&m_buffer);
        return m_buffer.str();
    }

private:
    auto
    overflow(int_type c_) -> int_type override {
        return m_buffer.sputc(std::stringbuf::traits_type::to_char_type(c_));
    }

    auto
    xsputn(char const* s_, std::streamsize n_) -> std::streamsize override {
        return m_buffer.sputn(s_, n_);
    }

    std::stringbuf m_buffer;  ///< Buffer to store captured output.
};

/**
 * Streambuffer proxy to capture everything sent to a stream in a multithreaded context with
 * OpenMP. Every thread has its own underlying buffer, thus output is distinct to each thread.
 */
class streambuf_proxy_omp : public streambuf_proxy
{
/// Get the buffer for the current thread.
#define TPP_INTERN_CURRENT_THREAD_BUFFER() (m_thd_buffers[static_cast<std::size_t>(omp_get_thread_num())])

public:
    /**
     * Replace the underlying buffer of the stream.
     * As long as this object lives, everything sent to be stream is captured.
     *
     * @param stream_ is the stream to capture from.
     */
    explicit streambuf_proxy_omp(std::ostream& stream_)
        : streambuf_proxy(stream_), m_thd_buffers(static_cast<std::size_t>(omp_get_max_threads())) {}

    auto
    str() -> std::string override {
        auto&                   buf = TPP_INTERN_CURRENT_THREAD_BUFFER();
        clearer<std::stringbuf> _(&buf);
        return buf.str();
    }

private:
    auto
    overflow(int_type c_) -> int_type override {
        return TPP_INTERN_CURRENT_THREAD_BUFFER().sputc(std::stringbuf::traits_type::to_char_type(c_));
    }

    auto
    xsputn(char const* s_, std::streamsize n_) -> std::streamsize override {
        return TPP_INTERN_CURRENT_THREAD_BUFFER().sputn(s_, n_);
    }

    std::vector<std::stringbuf> m_thd_buffers;  ///< Buffer to store captured output.
};
}  // namespace test
}  // namespace intern
}  // namespace tpp

#ifndef _OPENMP
#    undef omp_get_max_threads
#    undef omp_get_thread_num
#endif
#undef TPP_INTERN_CURRENT_THREAD_BUFFER

#endif  // TPP_TEST_STREAMBUF_PROXY_HPP
