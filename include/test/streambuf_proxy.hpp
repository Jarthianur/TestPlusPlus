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

#include <mutex>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <vector>

#include "omp.hpp"

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

    explicit streambuf_proxy(std::ostream& stream_) : m_orig_buf(stream_.rdbuf(this)), m_orig_stream(stream_) {}

    ~streambuf_proxy() noexcept override {
        m_orig_stream.flush();
        m_orig_stream.rdbuf(m_orig_buf);
    }

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

    std::streambuf* m_orig_buf;
    std::ostream&   m_orig_stream;
};

class streambuf_proxy_single : public streambuf_proxy
{
public:
    explicit streambuf_proxy_single(std::ostream& stream_) : streambuf_proxy(stream_) {}

    auto
    str() -> std::string override {
        clearer<std::stringbuf> _(&m_buffer);
        m_orig_stream.flush();
        return m_buffer.str();
    }

private:
    auto
    overflow(int_type c_) -> int_type override {
        std::lock_guard<std::mutex> lk(m_mutex);
        return m_buffer.sputc(std::stringbuf::traits_type::to_char_type(c_));
    }

    auto
    xsputn(char const* s_, std::streamsize n_) -> std::streamsize override {
        std::lock_guard<std::mutex> lk(m_mutex);
        return m_buffer.sputn(s_, n_);
    }

    std::stringbuf m_buffer;
    std::mutex mutable m_mutex;
};

class streambuf_proxy_omp : public streambuf_proxy
{
#define TPP_INTERN_CURRENT_THREAD_BUFFER() (m_thd_buffers[static_cast<std::size_t>(omp_get_thread_num())])

public:
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

    std::vector<std::stringbuf> m_thd_buffers;

#undef TPP_INTERN_CURRENT_THREAD_BUFFER
};
}  // namespace test
}  // namespace intern
}  // namespace tpp

#endif  // TPP_TEST_STREAMBUF_PROXY_HPP
